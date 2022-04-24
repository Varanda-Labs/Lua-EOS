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

mos_thread_h_t mos_thread_new(  const char *pcName, 
                                thread_func_t thread_func, 
                                void *pvArg, 
                                uint32_t iStackSize, 
                                uint32_t iPriority )
{
  TaskHandle_t xCreatedTask;
  portBASE_TYPE xResult;
  mos_thread_h_t xReturn;

  mos_thread_h_t xReturn = rt_thread_create ( pcName,
		  	                                      thread_func, /* void(*)(void *parameter) */
		                                          pvArg,
		                                          iStackSize,
		  	                                      iPriority,
                                              20);

  return xReturn;
}

void mos_thread_sleep( uint32_t time_milliseconds)
{
  if (time_milliseconds == 0) {
    rt_thread_yield ();
  }
  else {
    rt_thread_sleep (rt_tick_from_millisecond(time_milliseconds));
  }
}

void mos_thread_delete(mos_thread_h_t thread)
{
  rt_thread_delete(thread);
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

  mos_timer_int_ptr_t this_timer = (mos_timer_int_ptr_t) pvTimerGetTimerID(native_timer_h);
  if (this_timer->magic != TIMER_MAGIC_WORD) {
    LOG_E("bad timer");
    return;
  }

  if (  (this_timer->type == MOS_TIMER_TYPE__SINGLE) ||
        (this_timer->type == MOS_TIMER_TYPE__SINGLE_SLOW)) {
    rt_timer_delete(this_timer->h);
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
  //     rt_timer_delete(this_timer->h);
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
  static int cnt = 0;
  char name[16];
  snprintf(name, sizeof(name), "timer_%d", ++cnt);

  mos_timer_int_ptr_t this_timer = MOS_MALLOC(sizeof(struct mos_timer_int_st));
  if ( ! this_timer ) {
    LOG_E("mos_timer_create_single_shot: no memo");
    return NULL;
  }

  rt_uint8_t flag = RT_TIMER_FLAG_SOFT_TIMER;
  if (periodic) flag |= RT_TIMER_FLAG_PERIODIC;
  else flag |= RT_TIMER_FLAG_ONE_SHOT;

  this_timer->h = rt_timer_create(  name,
		                                internal_timer_callback, /*void(*)(void *parameter) */ 	
		                                (void *) this_timer,
		                                rt_tick_from_millisecond (time_milliseconds),
		                  	            flag);

  if ( ! this_timer->h ) {
    MOS_FREE(this_timer);
    LOG_W("mos_timer_create_single_shot: fail to create native timer");
    return NULL;
  }
  this_timer->type = tm_type;
  this_timer->user_callback = (void *) callback;
  this_timer->user_arg = arg;
  this_timer->pending_destruction = false;

  if (rt_timer_start(this_timer->h) != RT_EOK) {
    rt_timer_delete(this_timer->h);
    MOS_FREE(this_timer);
    LOG_W("mos_timer_create_single_shot: fail to start native timer");
    return NULL;
  }
  this_timer->magic = TIMER_MAGIC_WORD;
  return this_timer;
}

bool mos_timer_create_single_shot( uint32_t time_milliseconds, timer_func_t callback, mos_timer_id_t id )
{
  return (_timer_create( time_milliseconds, callback, (void *) id, false, MOS_TIMER_TYPE__SINGLE ) != NULL);
}



#ifdef __cplusplus
  }
#endif


