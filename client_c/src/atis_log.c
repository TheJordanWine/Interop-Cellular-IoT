/* 
 *	SPDX-License-Identifier:	BSD-3-Clause 
 * The above license addresses copyright obligations. Please refer to Section 10.4 of the 
 * ATIS Operating Procedures for information about patent licensing obligations under 
 * ATIS' Intellectual Property Rights Policy.
 */

#include "atis_log.h"

#define ATIS_LOG_UNUSED(x) (void)(x)

qapi_UART_Handle_t atis_log_uart_hdlr = NULL;
TX_MUTEX * atis_log_mutex;


void atis_log_output(const char* s)
{
	if (atis_log_uart_hdlr) 
	{
		// This may be called from different threads, so use a mutex to
		// give only one thread control at a time.
		// Note this is blocking - threads will stall until they can get access.
		tx_mutex_get(atis_log_mutex, TX_WAIT_FOREVER);
		qapi_UART_Transmit(atis_log_uart_hdlr, s, strlen(s), NULL);
		// Wait for call-back to show buffer is empty before continuing
		// See 80-p8101-32 section 16.3.4.4 - "buffer is owned by UART driver
		// until tx_cb_isr is called
		
		// Delay is calculated on approx 0.1ms/character at 115200 baud + 10ms
		// of latency
		qapi_Timer_Sleep(10+strlen(s)/10, QAPI_TIMER_UNIT_MSEC, true);
		qapi_UART_Transmit(atis_log_uart_hdlr, "\r\n", strlen("\r\n"), NULL);
		qapi_Timer_Sleep(10, QAPI_TIMER_UNIT_MSEC, true);
		tx_mutex_put(atis_log_mutex);
	}
}

static void atis_log_uart_rx_cb(uint32_t num_bytes, void *cb_data)
{
	ATIS_LOG_UNUSED(num_bytes);
	ATIS_LOG_UNUSED(cb_data);
}

static void atis_log_uart_tx_cb(uint32_t num_bytes, void *cb_data)
{
	ATIS_LOG_UNUSED(num_bytes);
	ATIS_LOG_UNUSED(cb_data);
}

void atis_log_init_uart()
{
	
	qapi_Status_t status;
	qapi_UART_Open_Config_t uart_cfg;
	int ret;

	uart_cfg.baud_Rate			= 115200;
	uart_cfg.enable_Flow_Ctrl	= QAPI_FCTL_OFF_E;
	uart_cfg.bits_Per_Char		= QAPI_UART_8_BITS_PER_CHAR_E;
	uart_cfg.enable_Loopback 	= 0;
	uart_cfg.num_Stop_Bits		= QAPI_UART_1_0_STOP_BITS_E;
	uart_cfg.parity_Mode 		= QAPI_UART_NO_PARITY_E;
	uart_cfg.rx_CB_ISR			= (qapi_UART_Callback_Fn_t)&atis_log_uart_rx_cb;
	uart_cfg.tx_CB_ISR			= (qapi_UART_Callback_Fn_t)&atis_log_uart_tx_cb;

	status = qapi_UART_Open(&atis_log_uart_hdlr, QAPI_UART_PORT_002_E, &uart_cfg);
	QAPI_MSG_SPRINTF(MSG_SSID_LINUX_DATA, MSG_LEGACY_MED, "atis_log_UART_Open status %d", status);

	status = qapi_UART_Power_On(atis_log_uart_hdlr);
	QAPI_MSG_SPRINTF(MSG_SSID_LINUX_DATA, MSG_LEGACY_MED, "atis_log_UART_Power_On status %d", status);
	
	txm_module_object_allocate(&atis_log_mutex, sizeof(TX_MUTEX));
	tx_mutex_create(atis_log_mutex, "atis_log_mutex", TX_NO_INHERIT);
	
}

void atis_log_release_uart()
{
	qapi_Status_t status;
	
	if (atis_log_uart_hdlr)
	{
		status = qapi_UART_Power_Off(atis_log_uart_hdlr);
		QAPI_MSG_SPRINTF(MSG_SSID_LINUX_DATA, MSG_LEGACY_MED, "atis_log_UART_Power_Off status %d", status);
		status = qapi_UART_Close(atis_log_uart_hdlr);
		QAPI_MSG_SPRINTF(MSG_SSID_LINUX_DATA, MSG_LEGACY_MED, "atis_log_UART_Close status %d", status);
		atis_log_uart_hdlr = NULL;
		tx_mutex_delete(atis_log_mutex);
		txm_module_object_deallocate(atis_log_mutex);
	}
}

void atis_format_log_msg( char *buf_ptr, size_t buf_size, char *fmt, ...)
{
	va_list ap;
	if( NULL == buf_ptr)
	{
		QAPI_MSG_SPRINTF(MSG_SSID_LINUX_DATA,MSG_LEGACY_ERROR,
						 "[DAM ERR]Bad Param buf_ptr:[%p], buf_size:%d",
						 buf_ptr, buf_size);
		atis_log_output("[DAM ERR]Bad Param buf_ptr");
		return;
	}

	va_start(ap, fmt);
	vsnprintf(buf_ptr, buf_size, fmt, ap);
	va_end(ap);

} 




