#include <stdlib.h>
#include <string.h>
#include <stdio.h>

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

int buildConfig() {
  FILE *fp;
  fp = fopen("/datatx/http_config.json", "w+"); // Open configuration file
  fprintf(fp,"{\n");
  fprintf(fp,"\t\"apn\": \"%s\",\n", apn);
  fprintf(fp,"\t\"apn_username\": \"%s\",\n", apn_username);
  fprintf(fp,"\t\"apn_password\": \"%s\",\n", apn_password);
  fprintf(fp,"\t\"use_https\": %d,\n", use_https);
  fprintf(fp,"\t\"use_psk\": %d,\n", use_psk);
  fprintf(fp,"\t\"server_address\": \"%s\",\n", server_address);
  fprintf(fp,"\t\"server_port\": %d,\n", server_port);
  fprintf(fp,"\t\"server_path\": \"%s\",\n", server_path);
  fprintf(fp,"\t\"ae_from_id\": \"%s\",\n", ae_from_id);
  fprintf(fp,"\t\"ae_resource_name\": \"%s\",\n", ae_resource_name);
  fprintf(fp,"\t\"container_resource_name\": \"%s\",\n", container_resource_name);
  fprintf(fp,"\t\"app_id\": \"%s\",\n", app_id);
  fprintf(fp,"\t\"dns_pri_v4\": \"%s\",\n", dns_pri_v4);
  fprintf(fp,"\t\"dns_sec_v4\": \"%s\",\n", dns_sec_v4);
  fprintf(fp,"\n}");
  fclose(fp); // Close configuration file
}
