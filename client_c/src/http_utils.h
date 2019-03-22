/*
 *	SPDX-License-Identifier:	BSD-3-Clause
 * The above license addresses copyright obligations. Please refer to Section 10.4 of the
 * ATIS Operating Procedures for information about patent licensing obligations under
 * ATIS' Intellectual Property Rights Policy.
 */


#ifndef http_utils_h
#define http_utils_h

#include "atis_log.h"
#include "qapi_fs.h"
#include "qapi_psm.h"
#include "qapi_dss.h"
#include "qapi_httpc.h"

#define MAX_FILE_SIZE 		2048

extern char file_buffer [MAX_FILE_SIZE+1];

qapi_Status_t read_file_to_buffer(const char * name, uint32 * file_len_p);

qapi_Status_t store_pem_ca_list(const char * name, const char * store_name);
qapi_Status_t store_psk_list(const char * name, const char * store_name);
void list_certs(qapi_Net_SSL_Cert_Type_t type);


#endif
