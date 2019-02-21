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
/**
   * Main function, program entry point.
   */
int main(int argc, char *argv[])
{
    //Request to the om2m Server
    getResource();
    
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
