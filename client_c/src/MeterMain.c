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
#include <stdlib.h>
#include <string.h>
#include "onem2m_http.h"

/**
  * Main function, program entry point.
  */
int main (int argc, char* argv[]) {

  // Start HTTP service
  onem2m_http_start();

  // TODO

} // End of main function
