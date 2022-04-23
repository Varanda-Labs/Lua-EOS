/****************************************************************************/
//
//  Module:          $URL: $
//  Created By:      Marcelo Varanda
//  Revision:        $Revision: $
//  Last Updated By: $Author: $
//  Last Updated:    $Date:  $
//
//  Description: Part of M-OS 
//
//  Copyright (c) 2021 Marcelo Varanda, all rights reserved.
//
//
/****************************************************************************/

/* mos_thread_new

  mos_queue_create
  mos_queue_get
  mos_queue_put
  mos_queue_waiting
  mos_timer_create_single_shot */

#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include "mos.h"

#ifdef __cplusplus
extern "C" {
#endif

void * mos_malloc (size_t size) { return malloc(size); }
void * mos_calloc (size_t num, size_t size) {
    void * p = mos_malloc(num * size);
    if (p) memset(p, 0, num * size);
    return p;
}
void mos_free (void * p) { return free(p); }

// mos_thread_h_t mos_thread_new( const char *pcName, thread_func_t thread_func, void *pvArg, uint32_t iStackSize, uint32_t iPriority )
// {
//     pthread_t thread;
//     int t = pthread_create(&thread, NULL,
//                               (void *(*) (void *)) thread_func, NULL);
//     if (t) {
//         return NULL;
//     }
//     return thread;
// }

//////////////// Tasks ///////////////

mos_thread_h_t mos_thread_new( const char *pcName, thread_func_t thread_func, void *pvArg, uint32_t iStackSize, uint32_t iPriority )
//mos_thread_h_t mos_thread_new( const char *pcName, thread_func_t thread_func, void *pvArg, int iStackSize, int iPriority )
{
  TaskHandle_t xCreatedTask;
  portBASE_TYPE xResult;
  mos_thread_h_t xReturn;

  xResult = xTaskCreate( thread_func, pcName, iStackSize, pvArg, iPriority, &xCreatedTask );

  if( xResult == MOS_PASS ) {
    xReturn = xCreatedTask;
  }
  else {
    xReturn = NULL;
  }

  return xReturn;
}

void mos_thread_sleep( uint32_t time_milliseconds)
{
  if (time_milliseconds == 0) {
    taskYIELD();
  }
  else {
    vTaskDelay(pdMS_TO_TICKS(time_milliseconds));
  }
}

void mos_thread_delete(mos_thread_h_t thread)
{
  vTaskDelete(thread);
}

//////////////// Queues ///////////////

mos_queue_h_t mos_queue_create ( uint32_t len, uint32_t item_size)
{
  static int cnt = 0;
  char name[16];
  snprintf(name, sizeof(name), "queue_%d", ++cnt);
  return (mos_queue_h_t) rt_mq_create(  name,
                                        item_size,
                                        len,
                                        RT_IPC_FLAG_PRIO);
}

int mos_queue_put (mos_queue_h_t mq, const void * buffer)
{
  return rt_mq_send_wait(mq, buffer, 0, 0);
}

int mos_queue_put_from_isr (mos_queue_h_t mq, const void * buffer)
{
  return rt_mq_send_wait(mq, buffer, 0, 0);
}

int mos_queue_waiting (mos_queue_h_t mq)
{
  return (int) rt_mq_num_msgs(mq);
}

int mos_queue_get (mos_queue_h_t mq, void *buffer, uint32_t timeout_milliseconds)
{
  return (int) rt_mq_recv(  mq,
                            buffer,
                            0,
                            timeout_milliseconds);
}

//----------------- Mutex ------------------
mos_mutex_h_t mos_mutex_create(void)
{
  static int cnt = 0;
  char name[16];
  snprintf(name, sizeof(name), "mutex_%d", ++cnt);

  return (mos_mutex_h_t) rt_mutex_create(name, 0);
}

void mos_mutex_lock(mos_mutex_h_t mutex)
{
  RT_ASSERT(rt_mutex_take(mutex, MOS_WAIT_FOREVER) == MOS_PASS);
}

void mos_mutex_unlock(mos_mutex_h_t mutex)
{
  RT_ASSERT(rt_mutex_release(mutex) == MOS_PASS);
}

void mos_mutex_destroy(mos_mutex_h_t mutex)
{
  RT_ASSERT(rt_mutex_delete(mutex) == MOS_PASS);
}

//------------------- timers ---------------------
#define TIMER_MAGIC_WORD 0xb45ae83c

typedef enum {
  MOS_TIMER_TYPE__SINGLE,
  MOS_TIMER_TYPE__PRESERVED,
  MOS_TIMER_TYPE__PERIODIC,
  MOS_TIMER_TYPE__SINGLE_SLOW = 10
} mos_timer_type_t;

typedef struct mos_timer_int_st {
  int             magic;  // validation
  int             type;
  TimerHandle_t   h;
  void *          user_callback;
  void *          user_arg;
  bool            pending_destruction;
  int             slow_timer_idx;
} * mos_timer_int_ptr_t;

static void internal_timer_callback( TimerHandle_t native_timer_h )
{
  timer_func_t callback;

  //mos_mutex_lock(timer_mutex); Mutex would prevent to have a callback deleting any timer. Lets trust that the pending_destruction can avoid using mutex

  mos_timer_int_ptr_t this_timer = (mos_timer_int_ptr_t) pvTimerGetTimerID(native_timer_h);
  if (this_timer->magic != TIMER_MAGIC_WORD) {
    LOG_E("bad timer");
    return;
  }

  if (  (this_timer->type == MOS_TIMER_TYPE__SINGLE) ||
        (this_timer->type == MOS_TIMER_TYPE__SINGLE_SLOW)) {
    xTimerDelete(this_timer->h, portMAX_DELAY);
    if (this_timer->pending_destruction == false) {
      callback = (timer_func_t) this_timer->user_callback;
      if (callback) {
        callback((mos_timer_id_t) this_timer->user_arg);
      }
    }
    this_timer->magic = 0; // prevent to have a garbage in the heap that looks like a good timer
    MOS_FREE(this_timer);
  }
  // else if (this_timer->type == MOS_TIMER_TYPE__PERIODIC) {
  //   if (this_timer->pending_destruction == false) {
  //     // preserved periodic timer... just call back
  //     callback = (timer_func_t) this_timer->user_callback;
  //     if (callback)
  //       callback(this_timer->user_arg);
  //   }
  //   else {
  //     xTimerDelete(this_timer->h, portMAX_DELAY);
  //     this_timer->magic = 0; // prevent to have a garbage in the heap that looks like a good timer
  //     MOS_FREE(this_timer);
  //   }

  // }
  else {
    LOG_E("Timer type not yet implemented");
  }
  //mos_mutex_unlock(timer_mutex);
}

                                    // ( time_milliseconds,     callback,              id,        false,          MOS_TIMER_TYPE__SINGLE )
static mos_timer_int_ptr_t _timer_create( int time_milliseconds, timer_func_t callback, void * arg, bool periodic, int tm_type )
{
  mos_timer_int_ptr_t this_timer = MOS_MALLOC(sizeof(struct mos_timer_int_st));
  if ( ! this_timer ) {
    LOG_E("mos_timer_create_single_shot: no memo");
    return NULL;
  }
  this_timer->h = xTimerCreate( NULL,                                 //const char *pcTimerName,
                                pdMS_TO_TICKS(time_milliseconds),     //const TickType_t xTimerPeriod,
                                periodic,                             //const UBaseType_t uxAutoReload,
                                (void *) this_timer,                  //void * const pvTimerID,
                                internal_timer_callback);             //TimerCallbackFunction_t pxCallbackFunction );
  if ( ! this_timer->h ) {
    MOS_FREE(this_timer);
    LOG_W("mos_timer_create_single_shot: fail to create native timer");
    return NULL;
  }
  this_timer->type = tm_type;
  this_timer->user_callback = (void *) callback;
  this_timer->user_arg = arg;
  this_timer->pending_destruction = false;

  if (xTimerStart(this_timer->h, portMAX_DELAY) != pdPASS) {
    xTimerDelete(this_timer->h, portMAX_DELAY);
    MOS_FREE(this_timer);
    LOG_W("mos_timer_create_single_shot: fail to start native timer");
    return NULL;
  }
  this_timer->magic = TIMER_MAGIC_WORD;
  return this_timer;
}

// mos_timer_id_t mos_timer_create_single_shot( int time_milliseconds, timer_func_t callback, void * arg )
// {
//   return _timer_create( time_milliseconds, callback, arg, false, mos_TIMER_TYPE__SINGLE );
// }

bool mos_timer_create_single_shot( uint32_t time_milliseconds, timer_func_t callback, mos_timer_id_t id )
{
  return (_timer_create( time_milliseconds, callback, (void *) id, false, MOS_TIMER_TYPE__SINGLE ) != NULL);
}



#ifdef __cplusplus
  }
#endif


