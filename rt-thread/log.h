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
#ifndef _LOG_H_
#define _LOH_H_

#if 1
#define LOG(fmt,...)   log_line((char *) "DEB",  __LINE__, (char *) fmt, ##__VA_ARGS__) /**< debug: its format is like printf */
#define LOG_I(fmt,...)   log_line((char *) "INF",  __LINE__, (char *) fmt, ##__VA_ARGS__) /**< debug: its format is like printf */
#define LOG_W(fmt,...)   log_line((char *) "WRN",  __LINE__, (char *) fmt, ##__VA_ARGS__) /**< debug: its format is like printf */
#define LOG_E(fmt,...)   log_line((char *) "ERR",  __LINE__, (char *) fmt, ##__VA_ARGS__) /**< debug: its format is like printf */
#else
#define LOG(...)         do {/* dummy */} while(0)
#define LOG_E(...)       do {/* dummy */} while(0)
#define LOG_I (...)       do {/* dummy */} while(0)
#define LOG_W(...)       do {/* dummy */} while(0)
#endif

//---------------------------------------------------
#endif
