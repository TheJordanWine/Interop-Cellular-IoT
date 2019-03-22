/*
 *	SPDX-License-Identifier:	BSD-3-Clause
 * The above license addresses copyright obligations. Please refer to Section 10.4 of the
 * ATIS Operating Procedures for information about patent licensing obligations under
 * ATIS' Intellectual Property Rights Policy.
 */

#include "http_utils.h"


char file_buffer [MAX_FILE_SIZE+1];


// Read file in to global buffer file_buffer
qapi_Status_t read_file_to_buffer(const char * name, uint32 * file_len_p)
{
	qapi_Status_t status;
	int file_handle;

	*file_len_p = 0;
	ATIS_LOG_INFO("Opening file %s", name);
	status = qapi_FS_Open(name, QAPI_FS_O_RDONLY_E, & file_handle);
	ATIS_LOG_INFO("File open result. Status = %d, fd = %d", status, file_handle);
	if (status!=QAPI_OK || file_handle == -1) {
		ATIS_LOG_ERROR("Can't open file %s", name);
		return (status!=QAPI_OK)?status:(qapi_Status_t)-1;
	}

	status = qapi_FS_Read(file_handle, & file_buffer, MAX_FILE_SIZE, file_len_p);
	ATIS_LOG_INFO("File read result. Status = %d", status);
	if (status!=QAPI_OK) {
		ATIS_LOG_ERROR("Can't read file %s", name);
		return status;
	}
	file_buffer[*file_len_p]=0;
	//Uncomment to output contents of file to debug stream
	//atis_log_output(file_buffer);
	status = qapi_FS_Close(file_handle);
	ATIS_LOG_INFO("QAPI file close result =%d", status);

	return QAPI_OK; // success
}



// Store the ca_list in .pem format in file name "name" in to the
// secure store under "store_name"
qapi_Status_t store_pem_ca_list(const char * name, const char * store_name)
{
	qapi_Status_t status;
	uint32 len;
	qapi_Net_SSL_Cert_Info_t cert_info;
	qapi_NET_SSL_CA_Info_t ca_info;

	cert_info.info.ca_List.ca_Info[0] = &ca_info;
	cert_info.info.ca_List.ca_Cnt = 1;
	cert_info.cert_Type = QAPI_NET_SSL_CA_LIST_E;

	status = read_file_to_buffer(name, & len);
	ATIS_LOG_INFO("Read PEM file for CA status = %d", status);
	if (status!=QAPI_OK)
		return status;

	ca_info.ca_Size = len;
	ca_info.ca_Buf = file_buffer;

	status = qapi_Net_SSL_Cert_Convert_And_Store(&cert_info, store_name);
	ATIS_LOG_INFO("Convert and Store CA List. Status = %d", status);
	return status;
}

// Store the psk_list in QAPI format in file name "name" in to the
// secure store under "store_name"
qapi_Status_t store_psk_list(const char * name, const char * store_name)
{
	qapi_Status_t status;
	uint32 len;
	qapi_Net_SSL_Cert_Info_t cert_info;

	cert_info.cert_Type = QAPI_NET_SSL_PSK_TABLE_E;

	status = read_file_to_buffer(name, & len);
	ATIS_LOG_INFO("Read PSK file status = %d", status);
	if (status!=QAPI_OK) {
		ATIS_LOG_ERROR("Can't read file %s", name);
		return status;
	}

	cert_info.info.psk_Tbl.psk_Size = len;
	cert_info.info.psk_Tbl.psk_Buf = file_buffer;

	status = qapi_Net_SSL_Cert_Convert_And_Store(&cert_info, store_name);
	ATIS_LOG_INFO("Convert and Store PSK key. Status = %d", status);
	return status;
}

// List resources in the secure store to the debug output of the type specified
void list_certs(qapi_Net_SSL_Cert_Type_t type)
{
	qapi_Net_SSL_Cert_List_t certs;
	qapi_Status_t status;
	ATIS_LOG_INFO("Getting list of certs of type %d", type);
	status = qapi_Net_SSL_Cert_List(type, & certs);
	ATIS_LOG_INFO("Got certs status = %d", status);
	for(int i = 0; i<status; i++)
		ATIS_LOG_INFO("Cert #%d, Name:%s", i, &((certs.name)[i][0]));
}
