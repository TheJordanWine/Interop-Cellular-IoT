/*
 *	SPDX-License-Identifier:	BSD-3-Clause
 * The above license addresses copyright obligations. Please refer to Section 10.4 of the
 * ATIS Operating Procedures for information about patent licensing obligations under
 * ATIS' Intellectual Property Rights Policy.
 */

 /**
  * Meter Main Application File for C:  MeterMain.c
  *
  * This file is the main source code file for the client application.
  * It utilizes the OS-IoT-BG96 C library to communicate using oneM2M
  * standards for a cellular network.  This class represents the Utility
  * Meter reader and stores values related to such.
  *
  * Instructions:
  *     See docs/client.md for the instructions on how to compile and execute
  *         this client application.
  *
  * @author Team 6
  *
  * @version 1.0
  */


// Operation is configured with the files "http_config.json" and "security_onboard.json" - see below

#include "qapi_dss.h"
#include "qapi_httpc.h"
#include "qapi_timer.h"
#include "qapi_dnsc.h"
#include "atis_log.h"
#include "qapi_fs.h"
#include "onem2m_http.h"
#include "txm_module.h"
#include "qapi_psm.h"
#include <stdlib.h>
#include <string.h>
#include "http_demo_utils.h"

// Configuration file for the application - see list of keys below
#define HTTP_CONFIG_FILE	"/datatx/http_config.json"

// Optional configuration file to onboard security credentials - see list of keys below
// If HTTPS is to be used then either a CA list or a PSK table (as appropriate)
// must be onboarded to the BG96's secure storage. Security credentials
// to be onboarded can be stored as a file on the BG96 and then added to the
// security_onboard.json file. When the application is started then files
// listed in the security_onboard.json will be onboarded and the security
// credentials will be available to the application.
#define SECURITY_ONBOARD_FILE "/datatx/security_onboard.json"

#define PSK_STORE_NAME      "psk_table.bin"
#define CA_STORE_NAME       "root.bin"

// http_config.json KEYS

// The following keys identify configurations that may be put in the
// file http_config.json to configure the application behavior
#define APN_KEY "apn" // Required string . APN is Access Point Name
#define APN_USERNAME_KEY "apn_username" // Required string
#define APN_PASSWORD_KEY "apn_password" // Required string
#define USE_HTTPS_KEY	"use_https" // Required boolean. Set to true for https or false for http
#define USE_PSK_KEY		"use_psk" // Required boolean. Read if use_https is true. Set to true for PSK or false for certificate HTTPS
#define SERVER_ADDRESS_KEY "server_address" // Required string. IP address or DNS name for CSE
#define SERVER_PORT_KEY "server_port" // Required number. Port number on CSE
#define SERVER_PATH_KEY "server_path" // Required string. OneM2M address for CSE base
#define AE_FROM_ID_KEY "ae_from_id" // Required string. "From" value (X-M2M-Origin) for AE
#define AE_RESOURCE_NAME_KEY "ae_resource_name" // Required string. Resource name to be used for the AE
#define CONTAINER_RESOURCE_NAME_KEY "container_resource_name" // Required string. Resource name to be used for the container.
#define APP_ID_KEY "app_id" // Required string. App-ID to be used for the AE resource
#define DNS_PRI_V4_KEY "dns_pri_v4" // Optional string. Include to set DNS primary server address (if DNS is used).
#define DNS_SEC_V4_KEY "dns_sec_v4" // Optional string. Include to set DNS secondary server address.

// The following keys identify onboard information that may be put in the file
// security_onboard.json
#define NO_DELETE_ONBOARD_FILE_KEY "no_delete_onboard_file" // Optional boolean.
// By default, when the security_onboard.json will be deleted after it has been processed. Include this value set to true to suppress this behaviour.
#define CA_PEM_FILE_NAME_KEY "ca_pem_file_name" // Optional String.
// If present the onboarding will attempt to import a CA from the file
// NB: file must be complete path: e.g. "/datatx/root.pem"
#define PSK_FILE_NAME_KEY "psk_file_name" // Optional String.
// If present the onboarding will attempt to import a PSK from the file
// NB: file must be complete path: e.g. "/datatx/psk.txt"
#define DELETE_REF_FILES_KEY "delete_ref_files" // Optional boolean
// If present and set to true the application will delete the source files of the security credentials following a successful onboarding.

// In this client, the application is interned to report data to the CSE by creating
// a new contentInstance resource roughly every REQUEST_INTERVAL seconds.
//
// In this client, failures are handled by making an initial set of retries in a small time
// window. If these retries fail then the attempt to make a report is abandoned and
// a new attempt will be made at the next scheduled report time.

#define DATA_CALL_ATTEMPTS 2 // Number of times to attempt to setup a data call before waiting until next scheduled report.
#define DATA_CALL_RETRY_INTERVAL 20 // Interval (seconds) for initial retries if data call fails
#define HTTP_ATTEMPTS 2 // Number of times to attempt to setup a data call before waiting until next scheduled report.
#define HTTP_RETRY_INTERVAL 20 // Interval (seconds) for initial retries if http request fails
#define REQUEST_INTERVAL 60 // Time (seconds) between reports

// Size of byte_pool used for dynamic memory - mostly needed for JSON encoding/decoding
// This size has been configured for demo purposes.
// This size will be more than ample and could likely be reduced.
#define BYTE_POOL_SIZE       (64000)


#define MAX_CONTENT_LEN	80 // Maximum length of content of a contentInstance resource

#define USE_PSM

typedef struct
{
  int   err;
  char *err_name;
} psm_err_map_type;

const psm_err_map_type reject_string[] = {
    {PSM_REJECT_REASON_NONE, "NONE"},
    {PSM_REJECT_REASON_NOT_ENABLED, "PSM FEATURE NOT ENABLED"},
    {PSM_REJECT_REASON_MODEM_NOT_READY, "MODEM NOT READY FOR PSM"},
    {PSM_REJECT_REASON_DURATION_TOO_SHORT, "ENTERED PSM DURATION IS TOO SHORT"},
};

const psm_err_map_type status_string[] = {
    {PSM_STATUS_NONE,               "NONE"},
    {PSM_STATUS_REJECT,             "REJECT"},
    {PSM_STATUS_READY,              "READY TO ENTER PSM MODE"},
    {PSM_STATUS_NOT_READY,          "NOT READY TO ENTER PSM MODE"},
    {PSM_STATUS_COMPLETE,           "ENTERED INTO PSM MODE"},
    {PSM_STATUS_DISCONNECTED,       "COMMUNICATION WITH SERVER LOST"},
    {PSM_STATUS_MODEM_LOADED,       "MODEM LOADED"},
    {PSM_STATUS_MODEM_NOT_LOADED,   "MODEM NOT LOADED"},
    {PSM_STATUS_NW_OOS,             "NW OOS"},
    {PSM_STATUS_NW_LIMITED_SERVICE, "NW LIMITED SERVICE"},
    {PSM_STATUS_HEALTH_CHECK,       "HEALTH CHECK"},
    {PSM_STATUS_FEATURE_ENABLED,    "PSM FEATURE ENABLED"},
    {PSM_STATUS_FEATURE_DISABLED,   "PSM FEATURE DISABLED"},
};

// State for master application
enum op_state_enum  {
	create_ae,
	create_container,
	reporting
};

typedef enum op_state_enum op_state_t;



// NB: **************************************************
// ** DO NOT MOVE THIS DEFINITION **
//
// Moving the location of this definition after the definition a variable of type op_state_t causes the program to hang
// when it decodes the config JSON (which will use the TX_BytePool. I suspect this is required to align
// to a memory word boundary and having a small (CHAR?) variable defined first breaks the alignment.
UCHAR   free_memory[BYTE_POOL_SIZE] = {0};

// Storage and identifiers for ThreadX events used to communicate between callback
// functions and the main thread in this application
TX_EVENT_FLAGS_GROUP * events_p;

const UINT EVENT_CALL_CONNECT=1;
const UINT EVENT_CALL_DISCONNECT=2;
const UINT EVENT_HTTP_SUCCESS=4;
const UINT EVENT_HTTP_FAIL=8;

// Handle to the application's DSS
qapi_DSS_Hndl_t	dss_h = NULL;

qapi_Net_SSL_Config_t ssl_config;


TX_BYTE_POOL * byte_pool_p;

onem2m_http_buffer_t http_cb_buffer;

// Global variable for keeping track of the Meter Value. Start at 0. 
int myMeterValue = 0; 

// Power Saving Mode (PSM) storage
psm_status_msg_type psm_status;
psm_info_type psm_info;
int32_t psm_client_id;

// Length of config file
uint32 config_len;
// JSON root for config data
JSON_Value *config_root = NULL;
// JSON object for config data
JSON_Object *config_object = NULL;
// Whether the config is set to use HTTPS (certificate or PSK based)
int config_use_https = 0;
// Whether the config it to use PSK (true) or Certificate based HTTPS (false).
// Only applicable if use_HTTP is true
int config_use_psk = 0;

int op_state = create_ae;

// Malloc alternative using ThreadX BytePool
void * http_malloc(size_t size)
{
	// Based on https://embeddedartistry.com/blog/2017/2/16/implementing-malloc-with-threadx
	void * p = NULL;
	if (size >0) {
		UINT tx_status = tx_byte_allocate(byte_pool_p, &p, size, TX_WAIT_FOREVER);
		if (tx_status != TX_SUCCESS) {
			ATIS_LOG_ERROR("Failure in http_malloc. Size requested = %d", size);
			p = NULL;
		}
	}
	return p;
}

// Free alternative using ThreadX BytePool
void http_free(void * p)
{
	if (p)
		UINT tx_status = tx_byte_release(p);
}

// Callback for Power Saving Mode (PSM)
void psm_cb(psm_status_msg_type * msg) {
	if (msg->status != PSM_STATUS_HEALTH_CHECK)
		ATIS_LOG_INFO( "PSM STATUS: %s; REASON %s",status_string[msg->status].err_name, reject_string[msg->reason].err_name);

	switch(msg->status) {
		case PSM_STATUS_HEALTH_CHECK:
			qapi_PSM_Client_Hc_Ack( msg -> client_id);
			break;
		default:
			break;
	}
}

// See if there is a security onboard file present, and if so process the content to
// onboard security credentials
static void onboard_security() {
	qapi_Status_t status;
	uint32 pos;
	JSON_Value *onboard_root = NULL;
	JSON_Object *onboard_object = NULL;
	int file_handle;
	int will_delete_file;
	int delete_ref;
	const char * ca_file_name = NULL;
	const char * psk_file_name = NULL;
	int success;

	status = read_file_to_buffer(SECURITY_ONBOARD_FILE, & pos);
	ATIS_LOG_INFO("SECURITY_ONBOARD_FILE read status: %d",status);
	if (status == QAPI_ERR_NO_ENTRY) {
		ATIS_LOG_INFO("No SECURITY_ONBOARD_FILE found. Continuing");
		return;
	} else if (status != QAPI_OK) {
		ATIS_LOG_ERROR("Error reading SECURITY_ONBOARD_FILE. Continuing without onboarding");
		return;
	}
	atis_log_output(file_buffer);
	ATIS_LOG_INFO("Parsing SECURITY_ONBOARD_FILE");
	onboard_root = json_parse_string(file_buffer);
	ATIS_LOG_INFO("Parsing result = %d", onboard_root);
	if (json_value_get_type(onboard_root) != JSONObject) {
		ATIS_LOG_ERROR("Didn't get JSONObject at root of SECURITY_ONBOARD_FILE file");
		json_value_free(onboard_root);
		return;
	}
	onboard_object = json_value_get_object(onboard_root);
	ATIS_LOG_INFO("Found JSONObject at root of SECURITY_ONBOARD_FILE file");
	delete_ref = json_object_get_boolean(onboard_object,DELETE_REF_FILES_KEY)==1;
	will_delete_file = json_object_get_boolean(onboard_object,NO_DELETE_ONBOARD_FILE_KEY)!=1;
	ca_file_name = json_object_get_string(onboard_object, CA_PEM_FILE_NAME_KEY);
	psk_file_name = json_object_get_string(onboard_object, PSK_FILE_NAME_KEY);
	json_value_free(onboard_root);
	success = 1;
	if (ca_file_name) {
		ATIS_LOG_INFO("Going to import CA: %s", ca_file_name);
		status = store_pem_ca_list(ca_file_name, CA_STORE_NAME);
		ATIS_LOG_INFO("Onboarding CA result: status = %d", status);
		if (status == QAPI_OK) {
			if (delete_ref) {
				status = qapi_FS_Unlink(ca_file_name);
				ATIS_LOG_INFO("Deleted file %s, status = %d", ca_file_name, status);
			}
		} else
			success = 0;
	}
	if (psk_file_name) {
		ATIS_LOG_INFO("Going to import PSK: %s", psk_file_name);
		status = store_psk_list(psk_file_name, PSK_STORE_NAME);
		ATIS_LOG_INFO("Onboarding PSK result: status = %d", status);
		if (status == QAPI_OK) {
			if (delete_ref) {
				status = qapi_FS_Unlink(psk_file_name);
				ATIS_LOG_INFO("Deleted file %s, status = %d", psk_file_name, status);
			}
		} else
			success = 0;
	}
	if (will_delete_file && success) {
		ATIS_LOG_INFO("Going to delete SECURITY_ONBOARD_FILE");
		status = qapi_FS_Unlink(SECURITY_ONBOARD_FILE);
		ATIS_LOG_INFO("Deleted file %s, status = %d", SECURITY_ONBOARD_FILE, status);
	}


}

static void set_ssl_config()
{
	if (config_use_psk) {
		ssl_config.cipher[0] = QAPI_NET_TLS_PSK_WITH_AES_256_CBC_SHA;
		ssl_config.cipher[1] = 0;
		ssl_config.cipher[2] = 0;
		ssl_config.cipher[3] = 0;
		ssl_config.cipher[4] = 0;
		ssl_config.cipher[5] = 0;
		ssl_config.cipher[6] = 0;
		ssl_config.cipher[7] = 0;
		// ssl_config.cipher[0] = QAPI_NET_TLS_PSK_WITH_AES_128_CBC_SHA;
		// ssl_config.cipher[1] = QAPI_NET_TLS_PSK_WITH_AES_256_CBC_SHA;
		// ssl_config.cipher[2] = QAPI_NET_TLS_PSK_WITH_AES_128_GCM_SHA256;
		// ssl_config.cipher[3] = QAPI_NET_TLS_PSK_WITH_AES_256_GCM_SHA384;
		// ssl_config.cipher[4] = QAPI_NET_TLS_PSK_WITH_AES_128_CBC_SHA256;
		// ssl_config.cipher[5] = QAPI_NET_TLS_PSK_WITH_AES_256_CBC_SHA384;
		// ssl_config.cipher[6] = 0;
		// ssl_config.cipher[7] = 0;
	} else {
		ssl_config.cipher[0] = QAPI_NET_TLS_RSA_WITH_AES_128_CBC_SHA;
		ssl_config.cipher[1] = QAPI_NET_TLS_RSA_WITH_AES_256_CBC_SHA;
		ssl_config.cipher[2] = QAPI_NET_TLS_ECDH_ECDSA_WITH_AES_256_CBC_SHA;
		ssl_config.cipher[3] = QAPI_NET_TLS_ECDH_ECDSA_WITH_AES_128_CBC_SHA;
		ssl_config.cipher[4] = QAPI_NET_TLS_ECDHE_ECDSA_WITH_AES_256_CBC_SHA;
		ssl_config.cipher[5] = QAPI_NET_TLS_ECDHE_RSA_WITH_AES_128_CBC_SHA;
		ssl_config.cipher[6] = QAPI_NET_TLS_ECDHE_RSA_WITH_AES_256_CBC_SHA;
		ssl_config.cipher[7] = QAPI_NET_TLS_ECDHE_RSA_WITH_AES_256_GCM_SHA384;
	}

	ssl_config.max_Frag_Len = 4096;
	ssl_config.max_Frag_Len_Neg_Disable = 0;
	ssl_config.protocol = QAPI_NET_SSL_PROTOCOL_TLS_1_2;
	ssl_config.verify.domain = 0;
	ssl_config.verify.match_Name[0] = '\0';
	ssl_config.verify.send_Alert = 1;
	ssl_config.verify.time_Validity = 0;	/* Don't check certification expiration */
}


static int set_data_call_params(const char * apn, const char * apn_username, const char * apn_passwd)
{
    qapi_DSS_Call_Param_Value_t param_info;

    if (NULL != dss_h)
    {
        /* set data call param */

		/* set apn */
        param_info.buf_val = apn;
        param_info.num_val = strlen(apn);
        ATIS_LOG_INFO("Setting APN - %s", apn);
        qapi_DSS_Set_Data_Call_Param(dss_h, QAPI_DSS_CALL_INFO_APN_NAME_E, &param_info);

		/* set apn username */
		param_info.buf_val = apn_username;
        param_info.num_val = strlen(apn_username);
        ATIS_LOG_INFO("Setting APN USER - %s", apn_username);
        qapi_DSS_Set_Data_Call_Param(dss_h, QAPI_DSS_CALL_INFO_USERNAME_E, &param_info);

		/* set apn password */
		param_info.buf_val = apn_passwd;
        param_info.num_val = strlen(apn_passwd);
        ATIS_LOG_INFO("Setting APN PASSWORD - %s", apn_passwd);
        qapi_DSS_Set_Data_Call_Param(dss_h, QAPI_DSS_CALL_INFO_PASSWORD_E, &param_info);

        /* set auth type */
		param_info.buf_val = NULL;
        param_info.num_val = 0;   // Don't use PAP/CHAP for auth. 
        ATIS_LOG_INFO("Setting AUTH - %d", param_info.num_val);
        qapi_DSS_Set_Data_Call_Param(dss_h, QAPI_DSS_CALL_INFO_AUTH_PREF_E, &param_info);

		/* set IP version(IPv4 or IPv6) */
        param_info.buf_val = NULL;
        param_info.num_val = QAPI_DSS_IP_VERSION_4;
        ATIS_LOG_INFO("Setting family to IPv%d", param_info.num_val);
        qapi_DSS_Set_Data_Call_Param(dss_h, QAPI_DSS_CALL_INFO_IP_VERSION_E, &param_info);
    }
    else
    {
        ATIS_LOG_ERROR("Dss handler is NULL!!!\n");
		return -1;
    }

    return 0;
}

// Callback for DSS events
// Will be notified of connect/disconnect events at the "data call" level.
// State changes are passed back to the main application using ThreadX events.
void dss_event_cb
(
	qapi_DSS_Hndl_t 		hndl,
	void 				   *user_data,
	qapi_DSS_Net_Evt_t 		evt,
	qapi_DSS_Evt_Payload_t *payload_ptr
)
{
	switch(evt) {
		case QAPI_DSS_EVT_NET_IS_CONN_E:
			ATIS_LOG_INFO("dss_event_cb, event: Connected");
			tx_event_flags_set(events_p, EVENT_CALL_CONNECT, TX_OR);
			break;
		case QAPI_DSS_EVT_NET_NO_NET_E:
			ATIS_LOG_INFO("dss_event_cb, event: Disconnect");
			tx_event_flags_set(events_p, EVENT_CALL_DISCONNECT, TX_OR);
			break;
		default:
		ATIS_LOG_INFO("dss_event_cb, event: %d", evt);
	}
}

// Callback for HTTP request results
// Information will be passed back to the main application using ThreadX events.
void http_client_cb(void* arg, int32 state, uint32_t http_resp_code, unsigned long int onem2m_rsc, onem2m_resource_t * res)
{
	ATIS_LOG_INFO("Application's http_client_cb. state = %d,,http_resp = %d, rsc = %d",state, http_resp_code, onem2m_rsc);
	if (state == QAPI_NET_HTTPC_RX_FINISHED ) {
		if (res) {
			ATIS_LOG_INFO("Resource type: %d",res -> onem2m_resource_type);
			onem2m_resource_free(res);
		}
		// Check for success response, or 4105 response if creating an AE or Container which
		// indicates that the AE already exists
		if (onem2m_rsc!=0 && (onem2m_rsc<=2999 || (op_state!=reporting && onem2m_rsc == ONEM2M_rcCONFLICT)))
			tx_event_flags_set(events_p, EVENT_HTTP_SUCCESS, TX_OR);
		else
			tx_event_flags_set(events_p, EVENT_HTTP_FAIL, TX_OR);
	} else { // Some kind of error at HTTP transaction level
		tx_event_flags_set(events_p, EVENT_HTTP_FAIL, TX_OR);
	}
}

qapi_Status_t create_onem2m_sess( void ) {
	qapi_Status_t status;

	onem2m_set_from( json_object_get_string(config_object, AE_FROM_ID_KEY) );
	if (config_use_https) {
		set_ssl_config();
		ATIS_LOG_INFO("Creating onem2m_http_session (HTTPS)");
		if (config_use_psk)
			status = onem2m_http_sess_start(&ssl_config, PSK_STORE_NAME, NULL, NULL,
			      byte_pool_p, NULL, http_client_cb);
		else
			status = onem2m_http_sess_start(&ssl_config, NULL, CA_STORE_NAME, NULL,
			         byte_pool_p, NULL, http_client_cb);
	} else {
		ATIS_LOG_INFO("Creating onem2m_http_session");
		status = onem2m_http_sess_start(NULL, NULL, NULL, NULL,
			         byte_pool_p, NULL, http_client_cb);
	}
	return status;
}

// Start data call and configure DNS
qapi_Status_t start_data_call( void ) {
	qapi_Status_t status;
	UINT tx_status;
	ULONG res_flags;

	tx_event_flags_set(events_p, ~(EVENT_CALL_CONNECT | EVENT_CALL_DISCONNECT), TX_AND);

	ATIS_LOG_INFO("Starting data call")
	status = qapi_DSS_Start_Data_Call(dss_h);
	ATIS_LOG_INFO("Data call result =%d",status);
	if (status != QAPI_OK)
		return status;

	ATIS_LOG_INFO("Waiting for data call to start");
	tx_status = tx_event_flags_get(events_p, EVENT_CALL_CONNECT | EVENT_CALL_DISCONNECT, TX_OR_CLEAR,
		                           & res_flags, TX_WAIT_FOREVER);

	if (tx_status != TX_SUCCESS || !(res_flags & EVENT_CALL_CONNECT)) {
		ATIS_LOG_INFO("Error connecting. Status = %d, flags = %d", tx_status, res_flags);
		return QAPI_ERROR;
	}
	// Start DNS and configure DNS servers if required by JSON config
	if (json_object_has_value_of_type(config_object, DNS_PRI_V4_KEY, JSONString)) {
		ATIS_LOG_INFO("Checking DNS Status")
		tx_status = qapi_Net_DNSc_Is_Started();
		ATIS_LOG_INFO(tx_status?"DNS Started":"DNS Not Started");
		if (!tx_status) {
			tx_status = qapi_Net_DNSc_Command(QAPI_NET_DNS_START_E);
			ATIS_LOG_INFO("Start DNS status (0=success) =%d", tx_status);
			if (tx_status) {
				ATIS_LOG_ERROR("Couldn't start DNS");
				return QAPI_ERROR;
			}
		}
		tx_status = qapi_Net_DNSc_Add_Server(json_object_get_string(config_object, DNS_PRI_V4_KEY), QAPI_NET_DNS_V4_PRIMARY_SERVER_ID);
		ATIS_LOG_INFO("Add primary DNS server (0=success) =%d", tx_status);
		if (tx_status) {
			ATIS_LOG_ERROR("Couldn't set primary DNS server");
			return QAPI_ERROR;
		}
		if (json_object_has_value_of_type(config_object, DNS_SEC_V4_KEY, JSONString)) {
			tx_status = qapi_Net_DNSc_Add_Server(json_object_get_string(config_object, DNS_SEC_V4_KEY), QAPI_NET_DNS_V4_SECONDARY_SERVER_ID);
			ATIS_LOG_INFO("Add secondary DNS server (0=success) =%d", tx_status);
			if (tx_status) {
				ATIS_LOG_ERROR("Couldn't set secondary DNS server");
				return QAPI_ERROR;
			}
		}
	}
	return QAPI_OK;
}

void stop_data_call( void ) {
	qapi_Status_t status;
	ATIS_LOG_INFO("Stopping Data Call");
	status = qapi_DSS_Stop_Data_Call(dss_h);
	ATIS_LOG_INFO("Data call stop result =%d",status);
}

// Wait for TX EVENT to indicate a response to an HTTP request
qapi_Status_t wait_for_http_session( void ) {
	UINT res_flags;
	ATIS_LOG_INFO("Waiting for HTTP request to respond");
	tx_event_flags_get(events_p, EVENT_HTTP_SUCCESS | EVENT_HTTP_FAIL, TX_OR_CLEAR,
		                           & res_flags, TX_WAIT_FOREVER);
	if (res_flags & EVENT_HTTP_SUCCESS)
		return QAPI_OK;
	else
		return QAPI_ERROR;
}

qapi_Status_t start_data_call_with_retry() {
	qapi_Status_t status;
	UINT i;
	for (i = 0; i< DATA_CALL_ATTEMPTS; i++) {
		ATIS_LOG_INFO("Try Establish Data call # %d", i+1);
		status = start_data_call();
		if (status == QAPI_OK)
			break;
		ATIS_LOG_INFO("Retry establishing data call");
		if (i<(DATA_CALL_ATTEMPTS-1))
			qapi_Timer_Sleep(DATA_CALL_RETRY_INTERVAL, QAPI_TIMER_UNIT_SEC, true);
		}
	return status;
}

qapi_Status_t http_connect_with_retry(const char * host, uint16_t port) {
	qapi_Status_t status;
	UINT i;
	for (i = 0; i< HTTP_ATTEMPTS; i++) {
		ATIS_LOG_INFO("Try HTTP Connecting # %d", i+1);
		status = onem2m_http_connect(host, port);
		if (status == QAPI_OK)
			break;
		if (i<(HTTP_ATTEMPTS-1))
			qapi_Timer_Sleep(HTTP_RETRY_INTERVAL, QAPI_TIMER_UNIT_SEC, true);
		}
	return status;
}

// Build the text string to post to the OM2M server. 
// Use the current reading of global variable myMeterValue.  
void get_content_string(char * str) {
	// Include the myMeterValue in a JSON format string.  
	snprintf(str, MAX_CONTENT_LEN, "{\"kWH\": %d}", myMeterValue);
}

// This function is used to increment the global variable myMeterValue.  
// It adds a random value from 0-20 to the current myMeterValue. 
void incrementMyMeterValue() {
	// Generate the random int between 0-20. 
	int r = rand() %20; 
	// Add the random value to myMeterValue.  
	myMeterValue = myMeterValue + r; 
}

// Send an appropriate HTTP request based on the current state
qapi_Status_t http_request_using_state() {
	qapi_Status_t status;
	char address [ONEM2M_MAX_ADDRESS_LEN];
	char content_string [MAX_CONTENT_LEN];
	onem2m_resource_t * res;
	strcpy(address, json_object_get_string(config_object,SERVER_PATH_KEY));
	switch (op_state) {
		case create_ae:
			res = onem2m_resource_allocate_AE();
			if (! res)
				return QAPI_ERROR;
			onem2m_AE_set_resourceName_string(res, json_object_get_string(config_object, AE_RESOURCE_NAME_KEY));
			onem2m_AE_set_requestReachability_boolean(res, 0);
			onem2m_AE_set_App_ID_string(res, json_object_get_string(config_object,APP_ID_KEY));
			status = onem2m_http_create(address, "1234", res, http_cb_buffer);
			ATIS_LOG_INFO("Create AE request result = %d", status);
			onem2m_resource_free(res);
			break;
		case create_container:
			strcpy(&(address[strlen(address)]),"/");
			strcpy(&(address[strlen(address)]), json_object_get_string(config_object, AE_RESOURCE_NAME_KEY));
			res = onem2m_resource_allocate_container();
			if (! res)
				return QAPI_ERROR;
			onem2m_container_set_resourceName_string(res, json_object_get_string(config_object, CONTAINER_RESOURCE_NAME_KEY));
			status = onem2m_http_create(address, "1234", res, http_cb_buffer);
			ATIS_LOG_INFO("Create container request result = %d", status);
			onem2m_resource_free(res);
			break;
		case reporting:
			strcpy(&(address[strlen(address)]),"/");
			strcpy(&(address[strlen(address)]), json_object_get_string(config_object, AE_RESOURCE_NAME_KEY));
			strcpy(&(address[strlen(address)]),"/");
			strcpy(&(address[strlen(address)]), json_object_get_string(config_object, CONTAINER_RESOURCE_NAME_KEY));
			res = onem2m_resource_allocate_contentInstance();
			if (! res)
				return QAPI_ERROR;
			onem2m_contentInstance_set_contentInfo_string(res, "application/text");
			get_content_string(content_string);
			ATIS_LOG_INFO("DEBUG: REPORTING TEXT: %s", content_string);
			onem2m_contentInstance_set_content_string(res, content_string);
			status = onem2m_http_create(address, "1234", res, http_cb_buffer);
			ATIS_LOG_INFO("Create ContentInstance request result = %d", status);
			onem2m_resource_free(res);
			break;
//
// Code useful for testing
//
// Update
		// onem2m_resource_t * res = onem2m_resource_allocate_AE();
		// onem2m_AE_init_labels(res);
		// onem2m_AE_append_labels_string(res,"testlabel");
		// status = onem2m_http_update("/in-cse/in-name/MyAE01", "1234", res, http_cb_buffer);
		// ATIS_LOG_INFO("Update result = %d", status);
		// onem2m_resource_free(res);

// Retrieve
		// status = onem2m_http_retrieve(address, "1234", http_cb_buffer);
		// ATIS_LOG_INFO("Retrieve result = %d", status);
	}

	return status;
}

qapi_Status_t http_request_with_retry() {
	qapi_Status_t status;
	UINT i;
	for (i = 0; i< HTTP_ATTEMPTS; i++) {
		ATIS_LOG_INFO("Try HTTP request # %d", i+1);
		status = http_request_using_state();
		if (status == QAPI_OK) {
			status = wait_for_http_session();
			if (status == QAPI_OK) {
				if (op_state != reporting)
					op_state ++;
				break;
			}
		}
		if (i<(HTTP_ATTEMPTS-1))
			qapi_Timer_Sleep(HTTP_RETRY_INTERVAL, QAPI_TIMER_UNIT_SEC, true);
	}
	return status;

}

qapi_Status_t register_psm() {
	qapi_Status_t status;
	ATIS_LOG_INFO("Registering PSM");
	status = qapi_PSM_Client_Register(&psm_client_id, psm_cb, & psm_status);
	ATIS_LOG_INFO("Register PSM Status = %d; client_id = %d", status, psm_client_id);
}

// Main function of the client
int run_ae()
{
	int ret = -1;
	qapi_Status_t status;
	UINT tx_status;

	atis_log_init_uart();
	qapi_Timer_Sleep(10, QAPI_TIMER_UNIT_SEC, true);
	ATIS_LOG_INFO("HTTP Example Start.");
	ATIS_LOG_INFO("Compiled at: %s", __TIME__);

#ifdef USE_PSM
	register_psm();
#endif

	do {
		ATIS_LOG_INFO("Allocating byte pool");
		tx_status = txm_module_object_allocate(&byte_pool_p, sizeof(TX_BYTE_POOL));
		ATIS_LOG_INFO("txm_module_object_allocate status=%d", tx_status);
		tx_byte_pool_create(byte_pool_p, "http application pool", free_memory, BYTE_POOL_SIZE);
		ATIS_LOG_INFO("tx_byte_pool_create status=%d", tx_status);

		ATIS_LOG_INFO("Allocating event flags");
		tx_status = txm_module_object_allocate(&events_p, sizeof(TX_EVENT_FLAGS_GROUP));
		ATIS_LOG_INFO("txm_module_object_allocate status=%d", tx_status);
		tx_event_flags_create(events_p, "ONEM2M EVENTS");
		onem2m_set_allocation_functions(http_malloc, http_free);

		onboard_security();

		// list_certs(QAPI_NET_SSL_CERTIFICATE_E);
		// list_certs(QAPI_NET_SSL_CA_LIST_E);
		// list_certs(QAPI_NET_SSL_PSK_TABLE_E);

		// store_psk_list(PSK_FILE_NAME, PSK_STORE_NAME);
		// list_certs(QAPI_NET_SSL_PSK_TABLE_E);

		// store_pem_ca_list(CA_PEM_FILE_NAME, CA_STORE_NAME);
		// list_certs(QAPI_NET_SSL_CA_LIST_E);

		status = read_file_to_buffer(HTTP_CONFIG_FILE, & config_len);
		ATIS_LOG_INFO("Parsing config file");
		config_root = json_parse_string(file_buffer);
		ATIS_LOG_INFO("Parsing result = %d", config_root);
		int got_values=0;
		if (json_value_get_type(config_root) != JSONObject) {
			ATIS_LOG_ERROR("Didn't get JSONObject at root of config file");
		} else {
			config_object = json_value_get_object(config_root);
			ATIS_LOG_INFO("Found JSONObject at root of config file");
			got_values = 1;
			got_values &= json_object_has_value_of_type(config_object, APN_KEY, JSONString);
			got_values &= json_object_has_value_of_type(config_object, APN_USERNAME_KEY, JSONString);
			got_values &= json_object_has_value_of_type(config_object, APN_PASSWORD_KEY, JSONString);
			got_values &= json_object_has_value_of_type(config_object, USE_HTTPS_KEY, JSONBoolean);
			got_values &= json_object_has_value_of_type(config_object, USE_PSK_KEY, JSONBoolean);
			got_values &= json_object_has_value_of_type(config_object, SERVER_ADDRESS_KEY, JSONString);
			got_values &= json_object_has_value_of_type(config_object, SERVER_PORT_KEY, JSONNumber);
			got_values &= json_object_has_value_of_type(config_object, SERVER_PATH_KEY, JSONString);
			got_values &= json_object_has_value_of_type(config_object, AE_FROM_ID_KEY, JSONString);
			got_values &= json_object_has_value_of_type(config_object, AE_RESOURCE_NAME_KEY, JSONString);
			got_values &= json_object_has_value_of_type(config_object, CONTAINER_RESOURCE_NAME_KEY, JSONString);
			got_values &= json_object_has_value_of_type(config_object, APP_ID_KEY, JSONString);
		}
		if (! got_values) {
			ATIS_LOG_ERROR("Couldn't read mandatory configuration values. Problem with config file.");
			break;
		}
		config_use_https = json_object_get_boolean(config_object, USE_HTTPS_KEY);
		config_use_psk = json_object_get_boolean(config_object, USE_PSK_KEY);

		ATIS_LOG_INFO("Init DSS API");
		status = qapi_DSS_Init(QAPI_DSS_MODE_GENERAL);
		ATIS_LOG_INFO("qapi_DSS_Init status=%d",status);
		if(status != QAPI_OK) {
			ATIS_LOG_INFO("qapi_DSS_Init returned error. Assuming DSS was already initialized by another process");
		}

		// Wait for DSS to initialize and register callback handler
		do {
			status = qapi_DSS_Get_Data_Srvc_Hndl(dss_event_cb, NULL, &dss_h);
			ATIS_LOG_INFO("dss_h %d, status %d\n", dss_h, status);

			if (dss_h)
				break;

			qapi_Timer_Sleep(100, QAPI_TIMER_UNIT_MSEC, true);
		} while (1);
		ATIS_LOG_INFO("GOT DSS Handle");

		set_data_call_params(
			json_object_get_string(config_object,APN_KEY),
			json_object_get_string(config_object,APN_USERNAME_KEY),
			json_object_get_string(config_object,APN_PASSWORD_KEY)
		);

		ATIS_LOG_INFO("Starting onem2m_http")
		status = onem2m_http_start();
		ATIS_LOG_INFO("onem2m_http_start result =%d",status);
		if(status != QAPI_OK) {
			ATIS_LOG_ERROR("Couldn't start HTTP");
			break;
		}

		status = create_onem2m_sess();
		ATIS_LOG_INFO("Creating onem2m_http_session result =%d", status);
		if(status != QAPI_OK) {
			ATIS_LOG_ERROR("Couldn't start http session");
			break;
		}

		int no_data_call = 1;
		UINT res_events;

		// Main loop to make repeated requests to the server.
		// Assumptions:
		// - HTTP session (create_onem2m_sess()) is reusable and not subject to errors
		// - Datacall failure will always be notified to us by the QAPI, so don't need to reestablish
		//   datacalls if we don't have any QAPI notifications.
		//
		// - The delay between request (REQUEST_INTERVAL) doesn't need to take account of delays caused
		//   by signaling and retries.
		do {
			// Make sure there is a data call up.
			if (no_data_call) {
				ATIS_LOG_INFO("no_data_call, so attempt to get one");
				status = start_data_call_with_retry();
				ATIS_LOG_INFO("Data call establishment result = %d", status);
				if (status == QAPI_OK) {
					no_data_call = 0;
				}
			} else {
				ATIS_LOG_INFO("Checking no loss of data call reported");
				tx_status = tx_event_flags_get(events_p, EVENT_CALL_DISCONNECT, TX_OR_CLEAR, &res_events, TX_NO_WAIT);
				if (tx_status == TX_SUCCESS && (res_events & EVENT_CALL_DISCONNECT)) {
					ATIS_LOG_INFO("Data call disconnected event detected. Setting up new data call.");
					status = start_data_call_with_retry();
					ATIS_LOG_INFO("Data call establishment result = %d", status);
				} else {
					ATIS_LOG_INFO("No loss of data call reported");
					status = QAPI_OK;
				}
			}

			// We think there is a data call, so let's connect to HTTP
			if (status == QAPI_OK) {
				ATIS_LOG_INFO("Connecting HTTP");
				status = http_connect_with_retry(
				json_object_get_string(config_object,SERVER_ADDRESS_KEY),
				(int)json_object_get_number(config_object,SERVER_PORT_KEY)
				);
				ATIS_LOG_INFO("HTTP connect status =%d",status);

				// We got an HTTP connection - let's try a request
				if (status == QAPI_OK) {
					int done_report = 0;
					do {
						if (op_state == reporting) {
							done_report = 1;
							incrementMyMeterValue();     // Increment the value of myMeterValue. 
						}
						ATIS_LOG_INFO("Doing HTTP request");
						status = http_request_with_retry();
						ATIS_LOG_INFO("HTTP Request result =%d",status);
					} while (status == QAPI_OK && ! done_report); // If results are successful then proceed through the initial states until we have done a report.
					onem2m_http_disconnect();
				} // Done HTTP request

			} // end of HTTP connection

#ifdef USE_PSM
			// Sleep until next request is due
			ATIS_LOG_INFO("Requesting PSM");
			psm_info.active_time_in_secs = 0;
			psm_info.psm_wakeup_type =  PSM_WAKEUP_MEASUREMENT_NW_ACCESS;
			psm_info.psm_time_info.time_format_flag = PSM_TIME_IN_SECS;
			psm_info.psm_time_info.psm_duration_in_secs = REQUEST_INTERVAL;
			status = qapi_PSM_Client_Enter_Psm(psm_client_id, &psm_info);
			ATIS_LOG_INFO("Psm Enter status = %d", status);
#endif
			// Sleep until next request is due
			qapi_Timer_Sleep(REQUEST_INTERVAL, QAPI_TIMER_UNIT_SEC, true);
		} while (1);

	} while (0);

	// NB Make sure no JSON configuration info is accessed after this point
	json_value_free(config_root);

	ATIS_LOG_INFO("Failure. Program stopped.");

	do {
		qapi_Timer_Sleep(10, QAPI_TIMER_UNIT_SEC, true);
	} while (1);

	atis_log_release_uart();

  return ret;
}
