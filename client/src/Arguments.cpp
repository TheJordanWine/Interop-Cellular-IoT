/**
 * Arguments Implementation File:  Arguments.cpp
 *
 * This file is used to implement the functions for the command
 * line arguments parsing of MeterMain.
 *
 * @author Team 6
 *
 * @version 1.0
 */

//Imports
#include <iostream>
#include <cstdlib>
#include <string>
#include "Arguments.h"
using namespace std;




/**
 * The Default Constructor for Arguments.  It assigns a default value of
 * "false" to the "successful" boolean class variable to indicate that
 * no parsing was performed on any input.
 */
Arguments::Arguments() {
  setDefaults();
}




/**
 * The Constructor for Arguments class.  It parses the input char[]
 * which represent the command line arguments of MeterMain.
 */
Arguments::Arguments(char ** argv, int argc) {
  setDefaults();
  parseFlags(argv, argc);   // Parse the command line args.
}




/**
 * This function parses the input char[] that represents
 * the command line arguments of MeterMain.  It will also
 * set different class variables based on what it finds during
 * the parsing activity.
 */
void Arguments::setDefaults() {
  successful = true;
  hostName = "127.0.0.1:8080";   // The IP:Port of OM2M server.
  loginCred = "admin:admin";     // The OM2M Server login credentials.
  aeName = "MY_METER";           // Name of the AE Resource to create.
  aeAppId = "app1";              // Name of the AE App Id. Mandatory.
  contName = "DATA";             // Data Container Name.
  location = "Home";             // Location of Utility Meter
  saveConfig = false;            // Don't save config.
  cseRootAddr = "/in-cse/in-name";      // SP-Relative address.
  secondsToDelay = 10;           // Seconds between meter-value updates
  runtime = 2;                   // Run for 2 minutes.
} // End of function setDefaults




/**
 * This function parses the input char[] that represents
 * the command line arguments of MeterMain.  It will also
 * set different class variables based on what it finds during
 * the parsing activity.
 */
void Arguments::parseFlags(char ** argv, int argc) {
  for (int i = 1; i < argc; i++) {
    if (argv[i][0] == '-') { // Check 1st character of arg for flag character
      if (strcmp(argv[i],"-a") == 0) { // aeAppId flag
        cout << "\nCommand line arg passed for AE App Id: ";
        cout << argv[i+1] << endl;
        if (vc.isValidName(argv[i+1])) { // Verify proper format
          aeAppId = argv[i+1];      // set aeAppId to the next argument
        }
        else {
          cout << "Invalid argument for AE App Id - " << argv[i+1]
          << "\n   Exiting...\n";
          successful = false;
          return;
        }
      }
      else if (strcmp(argv[i],"-c") == 0) { // contName flag
        cout << "\nCommand line arg passed for Container Name: ";
        cout << argv[i+1] << endl;
        if (vc.isValidName(argv[i+1])) { // Verify proper format
          contName = argv[i+1];       // set contName to the next argument
        }
        else {
          cout << "Invalid argument for Container Name - " << argv[i+1]
          << "\n   Exiting...\n";
          successful = false;
          return;
        }
      }
      else if (strcmp(argv[i],"-d") == 0) { // secondsToDelay flag
        cout << "\nCommand line arg passed for delay in seconds: ";
        cout << argv[i+1] << endl;
        if (vc.isValidInt(argv[i+1])) { // Verify proper format TODO
          secondsToDelay = atoi(argv[i+1]);    // set runtime to the next argument
        }
        else {
          cout << "Invalid argument for delay in seconds - " << argv[i+1]
          << "\n   Exiting...\n";
          successful = false;
          return;
        }
      }
      else if (strcmp(argv[i],"-h") == 0) { // hostName flag
        cout << "\nCommand line arg passed for OM2M server: ";
        cout << argv[i+1] << endl;
        if (vc.isValidIP(argv[i+1])) { // Verify proper format
          hostName = argv[i+1];    // set hostName to the next argument
        }
        else {
          cout << "Invalid argument for OM2M server - " << argv[i+1]
          << "\n   Exiting...\n";
          successful = false;
          return;
        }
      }
      else if (strcmp(argv[i],"-l") == 0) { // loginCred flag
        cout << "\nCommand line arg passed for login credentials: ";
        cout << argv[i+1] << endl;
        if (vc.isValidCred(argv[i+1])) { // Verify proper format
          loginCred = argv[i+1];       // set loginCred to the next argument
        }
        else {
          cout << "Invalid argument for login credentials - " << argv[i+1]
          << "\n   Exiting...\n";
          successful = false;
          return;
        }
      }
      else if (strcmp(argv[i],"-L") == 0) { // location flag * May change naming convention
        cout << "\nCommand line arg passed for location : ";
        cout << argv[i+1] << endl;
        if (vc.isValidName(argv[i+1])) { // Verify proper format
          location = argv[i+1];       // set loginCred to the next argument
        }
        else {
          cout << "Invalid argument for location - " << argv[i+1]
          << "\n   Exiting...\n";
          successful = false;
          return;
        }
      }
      else if (strcmp(argv[i],"-n") == 0) { // aeName flag
        cout << "\nCommand line arg passed for the AE Resource Name: ";
        cout << argv[i+1] << endl;
        if (vc.isValidName(argv[i+1])) { // Verify proper format
          aeName = argv[i+1];       // set aeName to the next argument
        }
        else {
          cout << "Invalid argument for AE Resource Name - " << argv[i+1]
          << "\n   Exiting...\n";
          successful = false;
          return;
        }
      }
      else if (strcmp(argv[i],"-r") == 0) { // cseRootAddr flag
        cout << "\nCommand line arg passed for the SP-Relative address: ";
        cout << argv[i+1] << endl;
        if (vc.isValidPath(argv[i+1])) {   // Verify proper format
          cseRootAddr = argv[i+1];      // set cseRootAddr to the next argument
        }
        else {
          cout << "Invalid argument for SP-Relative address - " << argv[i+1]
          << "\n   Exiting...\n";
          successful = false;
          return;
        }
      }
      else if (strcmp(argv[i],"-t") == 0) { // runtime flag
        cout << "\nCommand line arg passed for run-time in minutes: ";
        cout << argv[i+1] << endl;
        if (vc.isValidInt(argv[i+1])) { // Verify proper format
          runtime = atoi(argv[i+1]);    // set runtime to the next argument
        }
        else {
          cout << "Invalid argument for run-time in minutes - " << argv[i+1]
          << "\n   Exiting...\n";
          successful = false;
          return;
        }
      }
      else if (strcmp(argv[i],"-s") == 0) { // save configuration flag
        cout << "\nConfiguration will be saved: ";
        saveConfig = true;
      }
      else { // invalid flag
        cout << "Invalid flag  - " << argv[i]
        << "\n   Exiting...\n";
        successful = false;
        return;
      }
    }
  }
} // End of function parseFlags




/*
 * Getters for the private class variables.
 */
bool Arguments::getSuccessful() {
  return successful;
}
string Arguments::getHostName() {
  return hostName;
}
string Arguments::getLoginCred() {
  return loginCred;
}
string Arguments::getAeName() {
  return aeName;
}
string Arguments::getAeAppId() {
  return aeAppId;
}
string Arguments::getContName() {
  return contName;
}
string Arguments::getCseRootAddr() {
  return cseRootAddr;
}
string Arguments::getLocation() {
  return location;
}
double Arguments::getSecondsToDelay() {
  return secondsToDelay;
}
int Arguments::getRuntime() {
  return runtime;
}
bool Arguments::getSaveConfig() {
  return saveConfig;
}
