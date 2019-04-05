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
  return takeInput();
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
   strcpy(apn, json_object_get_string(config_object, APN_KEY) );
   strcpy(apn_username, json_object_get_string(config_object, APN_USERNAME_KEY) );
   strcpy(apn_password, json_object_get_string(config_object, APN_PASSWORD_KEY) );
   config_use_https = json_object_get_boolean(config_object, USE_HTTPS_KEY);
   use_psk = json_object_get_boolean(config_object, USE_PSK_KEY);
   strcpy(server_address, json_object_get_string(config_object, SERVER_ADDRESS_KEY) );
   strcpy(server_port, json_object_get_string(config_object, SERVER_PORT_KEY) );
   strcpy(server_path, json_object_get_string(config_object, SERVER_PATH_KEY) );
   strcpy(ae_from_id, json_object_get_string(config_object, AE_FROM_ID_KEY) );
   strcpy(ae_resource_name, json_object_get_string(config_object, AE_RESOURCE_NAME_KEY) );
   strcpy(strcpy(container_resource_name, json_object_get_string(config_object, CONTAINER_RESOURCE_NAME_KEY) );
   strcpy(app_id, json_object_get_string(config_object, APP_ID_KEY) );
   strcpy(dns_pri_v4, json_object_get_string(config_object, DNS_PRI_V4_KEY) );
   strcpy(dns_sec_v4, json_object_get_string(config_object, DNS_SEC_V4_KEY) );
   return 0;

}

int takeInput(){
  char menuInput[2];
  char input[32];
  while (true) {
    // Clear Inputs
    menuInput[0] = 0;
    menuInput[1] = 0;
    input[0] = 0
    printf("\nEnter a command:\n");
    printf("   1   | Edit %s | Current value: %s\n", APN_KEY, apn);
    printf("   2   | Edit %s | Current value: %s\n", APN_USERNAME_KEY, apn_username);
    printf("   3   | Edit %s | Current value: %s\n", APN_PASSWORD_KEY, apn_password);
    printf("   4   | Edit %s | Current value: %d\n", USE_HTTPS_KEY, use_https);
    printf("   5   | Edit %s | Current value: %d\n", USE_PSK_KEY, use_psk);
    printf("   6   | Edit %s | Current value: %s\n", SERVER_ADDRESS_KEY, server_address);
    printf("   7   | Edit %s | Current value: %d\n", SERVER_PORT_KEY, server_port);
    printf("   8   | Edit %s | Current value: %s\n", SERVER_PATH_KEY, server_path);
    printf("   9   | Edit %s | Current value: %s\n", AE_FROM_ID_KEY, ae_from_id);
    printf("  10   | Edit %s | Current value: %s\n", AE_RESOURCE_NAME_KEY, ae_resource_name);
    printf("  11   | Edit %s | Current value: %s\n", CONTAINER_RESOURCE_NAME_KEY, container_resource_name);
    printf("  12   | Edit %s | Current value: %s\n", APP_ID_KEY , app_id);
    printf("  13   | Edit %s | Current value: %s\n", DNS_PRI_V4_KEY, dns_pri_v4);
    printf("  14   | Edit %s | Current value: %s\n", DNS_SEC_V4_KEY, dns_sec_v4);
    printf("s or S | Save Configuration\n");
    printf("q or Q | Quit (Unsaved configuration will be lost)\n");
    fgets(menuInput,2,stdin);

    if (menuInput[0] == '0') {
      if (menuInput[1] == '1') {
        printf("Enter value for %s: ", APN_KEY)
        fgets(input,20,stdin);
        setAPN(&input[0]);
      }
      if (menuInput[1] == '2') {
        printf("Enter value for %s: ", APN_USERNAME_KEY)
        fgets(input,20,stdin);
        setUsername(&input[0]);
      }
      if (menuInput[1] == '3') {
        printf("Enter value for %s: ", APN_PASSWORD_KEY)
        fgets(input,20,stdin);
        setPassword(&input[0]);
      }
      if (menuInput[1] == '4') {
        printf("Enter value for %s: ", USE_HTTPS_KEY)
        fgets(input,20,stdin);
        setUseHTTPS(&input[0]);
      }
      if (menuInput[1] == '5') {
        printf("Enter value for %s: ", USE_PSK_KEY)
        fgets(input,20,stdin);
        setUsePSK(&input[0]);
      }
      if (menuInput[1] == '6') {
        printf("Enter value for %s: ", SERVER_ADDRESS_KEY)
        fgets(input,20,stdin);
        setAddress(&input[0]);
      }
      if (menuInput[1] == '7') {
        printf("Enter value for %s: ", SERVER_PORT_KEY)
        fgets(input,20,stdin);
        setPort(&input[0]);
      }
      if (menuInput[1] == '8') {
        printf("Enter value for %s: ", SERVER_PATH_KEY)
        fgets(input,20,stdin);
        setPath(&input[0]);
      }
      if (menuInput[1] == '9') {
        printf("Enter value for %s: ", AE_FROM_ID_KEY)
        fgets(input,20,stdin);
        setFrom(&input[0]);
      }

    }

    else if (menuInput[0] == '1') { // Can be 1 or 1x\
      if (menuInput[1] == '0') {
        printf("Enter value for %s: ", AE_RESOURCE_NAME_KEY)
        fgets(input,20,stdin);
        setResource(&input[0]);
      }
      if (menuInput[1] == '1') {
        printf("Enter value for %s: ", CONTAINER_RESOURCE_NAME_KEY)
        fgets(input,20,stdin);
        setContainer(&input[0]);
      }
      if (menuInput[1] == '2') {
        printf("Enter value for %s: ", APP_ID_KEY)
        fgets(input,20,stdin);
        setAppId(&input[0]);
      }
      if (menuInput[1] == '3') {
        printf("Enter value for %s: ", DNS_PRI_V4_KEY)
        fgets(input,20,stdin);
        setDnsPriV4(&input[0]);
      }
      if (menuInput[1] == '4') {
        printf("Enter value for %s: ", DNS_SEC_V4_KEY)
        fgets(input,20,stdin);
        setDnsSecV4(&input[0]);
      }
      else { // Assume 1
        printf("Enter value for %s: ", APN_KEY)
        fgets(input,20,stdin);
        setAPN(&input[0]);
      }
    }
    else if (menuInput[0] == '2') {
      printf("Enter value for %s: ", )
      fgets(input,20,stdin);
      setUsername(&input[0]);
    }
    else if (menuInput[0] == '3') {
      printf("Enter value for %s: ", )
      fgets(input,20,stdin);
      setPassword(&input[0]);
    }
    else if (menuInput[0] == '4') {
      printf("Enter value for %s: ", )
      fgets(input,20,stdin);
      setUseHTTPS(&input[0]);
    }
    else if (menuInput[0] == '5') {
      printf("Enter value for %s: ", )
      fgets(input,20,stdin);
      setUsePSK(&input[0]);
    }
    else if (menuInput[0] == '6') {
      printf("Enter value for %s: ", )
      fgets(input,20,stdin);
      setAddress(&input[0]);
    }
    else if (menuInput[0] == '7') {
      printf("Enter value for %s: ", )
      fgets(input,20,stdin);
      setPort(&input[0]);
    }
    else if (menuInput[0] == '8') {
      printf("Enter value for %s: ", )
      fgets(input,20,stdin);
      setPath(&input[0]);
    }
    else if (menuInput[0] == '9') {
      printf("Enter value for %s: ", )
      fgets(input,20,stdin);
      setFrom(&input[0]);
    }
    else if (menuInput[0] == 's' || menuInput[0] == 'S') { // Save configuration
      buildConfig();
    }
    else if (menuInput[0] == 'q' || menuInput[0] == 'Q') { // Quit configuration
      return 0;
    }
  }
}

void setAPN(char* input) {
  strcpy(apn,input);
}
void setUsername(char* input) {
  strcpy(apn_username,input);
}
void setPassword(char* input) {
  strcpy(apn_password,input);
}
void setUseHTTPS(char* input) {
  use_https = atoi(input);
}
void setUsePSK(char* input) {
  use_psk = atoi(input);
}
void setAddress(char* input) {
  strcpy(server_address,input);
}
void setPort(char* input) {
    server_port = atoi(input);
}
void setPath(char* input) {
  strcpy(server_path,input);
}
void setFrom(char* input) {
  strcpy(ae_from_id,input);
}
void setResource(char* input) {
  strcpy(ae_resource_name,input);
}
void setContainer(char* input) {
  strcpy(container_resource_name,input);
}
void setAppId(char* input) {
  strcpy(app_id,input);
}
void setDnsPriV4(char* input) {
  strcpy(dns_pri_v4,input);
}
void setDnsSecV4(char* input) {
  strcpy(dns_sec_v4,input);
}
