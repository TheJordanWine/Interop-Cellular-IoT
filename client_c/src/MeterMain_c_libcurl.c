/**
 * Example Hello World File for C:  ExampleHelloWorld.c
 *
 * This file is a simple Hello World example C program.  It is
 * used to demonstrate that the BG96 board can run a simple C
 * application.
 *
 * Instructions:
 *     1) Compile using gcc:
 *        gcc onem2m.c -lcurl
 *     2) Test, execute:
 *        ./a.out
 *     3) Copy to BG96 and execute. 
 *
 * @author Team 6
 *
 * @version 1.0
 */

// Imports
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>

// Forward Declrations 
void getResource();
void createMyMeter();
void createDescriptorContainer();
void createDataContainer();
/**
   * Main function, program entry point.
   */
int main(int argc, char *argv[])
{
    //Request to the om2m Server
    getResource();

    //Creates an AE MY_METER
    createMyMeter();
    
    //Creates a descriptor contianer for MY_METER
    createDescriptorContainer();

    //Creates a data container for MY_METER
    createDataContainer();
    
    return 0;
}

void getResource()
{
    printf("Getting resource....\n");
    CURL *hnd = curl_easy_init();

    curl_easy_setopt(hnd, CURLOPT_CUSTOMREQUEST, "GET");
    curl_easy_setopt(hnd, CURLOPT_URL, "http://127.0.0.1:8080/~/in-cse");

    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "x-m2m-origin: admin:admin");
    headers = curl_slist_append(headers, "accept: application/json");
    curl_easy_setopt(hnd, CURLOPT_HTTPHEADER, headers);

    CURLcode ret = curl_easy_perform(hnd);
    /* Perform the request, res will get the return code */
    /* Check for errors */
    if (ret != CURLE_OK)
    {
        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(ret));
    }

    /* always cleanup */
    curl_easy_cleanup(hnd);
}

void createMyMeter()
{
    printf("Creating my meter....\n");
    CURL *hnd = curl_easy_init();

    curl_easy_setopt(hnd, CURLOPT_CUSTOMREQUEST, "POST");
    curl_easy_setopt(hnd, CURLOPT_URL, "http://192.168.0.8:8080/~/in-cse");

    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "x-m2m-origin: admin:admin");
    headers = curl_slist_append(headers, "content-type: application/xml;ty=2");
    curl_easy_setopt(hnd, CURLOPT_HTTPHEADER, headers);

    curl_easy_setopt(hnd, CURLOPT_POSTFIELDS, "<m2m:ae xmlns:m2m=\"http://www.onem2m.org/xml/protocols\" rn=\"MY_METER\" ><api>app-sensor</api><lbl>Type/sensor Category/temperature Location/home</lbl><rr>false</rr></m2m:ae>");

    CURLcode ret = curl_easy_perform(hnd);
    if (ret != CURLE_OK)
    {
        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(ret));
    }

    /* always cleanup */
    curl_easy_cleanup(hnd);
}
void createDescriptorContainer()
{
    CURL *hnd = curl_easy_init();

    curl_easy_setopt(hnd, CURLOPT_CUSTOMREQUEST, "POST");
    curl_easy_setopt(hnd, CURLOPT_URL, "http://127.0.0.1:8080/~/in-cse/in-name/MY_METER");

    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "x-m2m-origin: admin:admin");
    headers = curl_slist_append(headers, "content-type: application/xml;ty=3");
    curl_easy_setopt(hnd, CURLOPT_HTTPHEADER, headers);

    curl_easy_setopt(hnd, CURLOPT_POSTFIELDS, "<m2m:cnt xmlns:m2m=\"http://www.onem2m.org/xml/protocols\" rn=\"DESCRIPTOR\"></m2m:cnt>");

    CURLcode ret = curl_easy_perform(hnd);
    if (ret != CURLE_OK)
    {
        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(ret));
    }

    /* always cleanup */
    curl_easy_cleanup(hnd);
}

void createDataContainer()
{
    CURL *hnd = curl_easy_init();

    curl_easy_setopt(hnd, CURLOPT_CUSTOMREQUEST, "POST");
    curl_easy_setopt(hnd, CURLOPT_URL, "http://127.0.0.1:8080/~/in-cse/in-name/MY_METER");

    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "x-m2m-origin: admin:admin");
    headers = curl_slist_append(headers, "content-type: application/xml;ty=3");
    curl_easy_setopt(hnd, CURLOPT_HTTPHEADER, headers);

    curl_easy_setopt(hnd, CURLOPT_POSTFIELDS, "<m2m:cnt xmlns:m2m=\"http://www.onem2m.org/xml/protocols\" rn=\"DATA\"></m2m:cnt>");

    CURLcode ret = curl_easy_perform(hnd);
    if (ret != CURLE_OK)
    {
        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(ret));
    }

    /* always cleanup */
    curl_easy_cleanup(hnd);
}