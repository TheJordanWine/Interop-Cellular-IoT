/* 
 *	SPDX-License-Identifier:	BSD-3-Clause 
 * The above license addresses copyright obligations. Please refer to Section 10.4 of the 
 * ATIS Operating Procedures for information about patent licensing obligations under 
 * ATIS' Intellectual Property Rights Policy.
 */

#ifndef _atis_log_h_
#define _atis_log_h_

#include "qapi_uart.h"
#include "qapi_timer.h"
#include "qapi_diag.h"
#include "qflog_utils.h"
#include <stdarg.h>
#include <stdio.h>

void atis_log_init_uart();

void atis_log_release_uart();

void atis_log_output( const char * s);

void atis_format_log_msg( char *buf_ptr, size_t buf_size, char *fmt, ...);

#define ATIS_LOG

/*

*/

/* Maximum length of log message */
// Note that a buffer this size is allocated on the stack. The stack size may be
// very small (1kbyte) so it is sensible to keep this value small.
#define MAX_LOG_MSG_SIZE       256

/* Log message to Diag */
// Enclosing the bock in do/while gives more compatibility for the expansion
// eg when inserted in an if statement.
#define ATIS_LOG_MESSAGE( level, ... )  		\
do {  											\
	char buf[MAX_LOG_MSG_SIZE];  				\
	atis_format_log_msg(buf, MAX_LOG_MSG_SIZE, ## __VA_ARGS__); \
	atis_log_output(buf);										\
} while (0);

#ifdef ATIS_LOG
/**
 *    @brief Info level logging macro.
 *
 *    Macro to log info.
 */
#define ATIS_LOG_INFO(...)  ATIS_LOG_MESSAGE(MSG_LEGACY_HIGH, ## __VA_ARGS__)

/**
 *    @brief Error level logging macro.
 *
 *    Macro to log errors.
 */
#define ATIS_LOG_ERROR(...) ATIS_LOG_MESSAGE(MSG_LEGACY_ERROR, ## __VA_ARGS__)

#else

#define ATIS_LOG_INFO(...)
#define ATIS_LOG_ERROR(...)

#endif


#endif
