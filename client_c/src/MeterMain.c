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

// Imports
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "onem2m_http.h"

// Set parameter values
const char* from = "FromValue";
const char* host = "127.0.0.1";
uint16_t port = 8080;

/**
  * Main function, program entry point.
  */
int main (int argc, char* argv[]) {

  // Start HTTP service
  onem2m_http_start();

  // Start an HTTP session
  onem2m_http_sess_start();

  // Configure the from value
  onem2m_set_from( from );

  // Start an HTTP connection
  onem2m_http_connect( host , port );

  // Create meter resource object

  // Perform oneM2M operations using onem2m_http_retrieve(),
  // onem2m_http_create(), and onem2m_http_update()

  // Use the callback function provided to process the results of the operation

  // Once the callback has been processed release the HTTP connection
  if (true) {
    onem2m_http_disconnect();
  }

  // HTTP sessions can be kept open for reuse or closed
  if (false) {
    onem2m_http_sess_end();

  }

} // End of main function
