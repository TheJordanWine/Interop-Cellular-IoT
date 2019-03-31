#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "http_utils.h"
#include "atis_log.h"

#define HTTP_CONFIG_FILE	"/datatx/http_config.json"

// The following keys identify configurations that may be put in the
// file http_config.json to configure the application behavior
#define APN_KEY "apn" // Required string
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


// Instantiate configuration parameters
char apn[64] = "AccessPointName";
char apn_username[64] = "admin";
char apn_password[64] = "admin";
bool use_https = 0;
bool use_psk = 0;
char server_address[64] = "127.0.0.1";
int server_port = 8080;
char server_path[64] = "/in-cse/in-name";
char ae_from_id[64] = "app1";
char ae_resource_name[64] = "MY_METER";
char container_resource_name[64] = "DATA";
char app_id[64] = "app1";
char dns_pri_v4[64] = "";
char dns_sec_v4[64] = "";

int run() {
  readconfig();
  buildconfig();
  return 0;
}

int buildConfig() {
  FILE *fp;
  fp = fopen(HTTP_CONFIG_FILE, "w+"); // Open configuration file
  // Write configuration to JSON file
  fprintf(fp,"{\n");
  fprintf(fp,"\t\"%s\": \"%s\",\n", APN_KEY, apn);
  fprintf(fp,"\t\"%s\": \"%s\",\n", APN_USERNAME_KEY, apn_username);
  fprintf(fp,"\t\"%s\": \"%s\",\n", APN_PASSWORD_KEY, apn_password);
  fprintf(fp,"\t\"%s\": %d,\n", USE_HTTPS_KEY, use_https);
  fprintf(fp,"\t\"%s\": %d,\n", USE_PSK_KEY, use_psk);
  fprintf(fp,"\t\"%s\": \"%s\",\n", SERVER_ADDRESS_KEY, server_address);
  fprintf(fp,"\t\"%s\": %d,\n", SERVER_PORT_KEY, server_port);
  fprintf(fp,"\t\"%s\": \"%s\",\n", SERVER_PATH_KEY, server_path);
  fprintf(fp,"\t\"%s\": \"%s\",\n", AE_FROM_ID_KEY, ae_from_id);
  fprintf(fp,"\t\"%s\": \"%s\",\n", AE_RESOURCE_NAME_KEY, ae_resource_name);
  fprintf(fp,"\t\"%s\": \"%s\",\n", CONTAINER_RESOURCE_NAME_KEY, container_resource_name);
  fprintf(fp,"\t\"%s\": \"%s\",\n", APP_ID_KEY, app_id);
  fprintf(fp,"\t\"%s\": \"%s\",\n", DNS_PRI_V4_KEY, dns_pri_v4);
  fprintf(fp,"\t\"%s\": \"%s\",\n", DNS_SEC_V4_KEY, dns_sec_v4);
  fprintf(fp,"\n}");
  fclose(fp); // Close configuration file
  return 0;
}

int readConfig() {

  // Read JSON file
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
    // Check for correct configuration format.
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

  // Get current configuration values from JSON file and assign
   strcpy(apn, json_object_get_string(config_object, USE_HTTPS_KEY) );
   strcpy(apn_username, json_object_get_string(config_object, USE_HTTPS_KEY) );
   strcpy(apn_password, json_object_get_string(config_object, USE_HTTPS_KEY) );
   config_use_https = json_object_get_boolean(config_object, USE_HTTPS_KEY);
   use_psk = json_object_get_boolean(config_object, USE_PSK_KEY);
   strcpy(server_address, json_object_get_string(config_object, USE_HTTPS_KEY) );
   strcpy(server_port, json_object_get_string(config_object, USE_HTTPS_KEY) );
   strcpy(server_path, json_object_get_string(config_object, USE_HTTPS_KEY) );
   strcpy(ae_from_id, json_object_get_string(config_object, USE_HTTPS_KEY) );
   strcpy(ae_resource_name, json_object_get_string(config_object, USE_HTTPS_KEY) );
   strcpy(strcpy(container_resource_name, json_object_get_string(config_object, USE_HTTPS_KEY) );
   strcpy(app_id, json_object_get_string(config_object, USE_HTTPS_KEY) );
   strcpy(dns_pri_v4, json_object_get_string(config_object, USE_HTTPS_KEY) );
   strcpy(dns_sec_v4, json_object_get_string(config_object, USE_HTTPS_KEY) );
   return 0;

}

int run() {
  readconfig();
  buildconfig();
  return 0;
}
