
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

#include "log.h"
#include "mos.h"
#include <stdio.h>

mos_mutex_h_t log_mutex;

#define LOCK()  mos_mutex_lock(log_mutex)
#define UNLOCK() mos_mutex_lock(log_mutex)

#define BUF_SIZE 256

void log_init(void)
{
  if ((log_mutex = mos_mutex_create()) == NULL) {
    printf("log_init: mutex failed\n");
  }
}

void log_line(char * what, int line, char * fmt, ...)
{
  char buffer[BUF_SIZE];

  va_list ap;
  va_start(ap, fmt); /* Initialize the va_list */

  vsnprintf (buffer, sizeof(buffer), fmt, ap);
  va_end(ap); /* Cleanup the va_list */

  LOCK();
  printf("%s (%d): %s\n", what, line, buffer);
  UNLOCK();

}
