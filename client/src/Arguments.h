/**
 * Arguments Header File:  Arguments.h
 *
 * This file is used to represent the interface for Arguments class.
 * This class contains functions and class variables that parse an
 * argument string for different values.
 *
 * @author Team 6
 *
 * @version 1.0
 */

#ifndef Arguments_H
#define Arguments_H

#include <iostream>
#include <cstdlib>
#include <string>
#include <string.h>
#include <sstream>
#include <fstream>
#include "ValidityCheck.h"

using namespace std;

class Arguments {

  public:

    Arguments();
    Arguments(char ** argv, int argc);

    bool getSuccessful();
    string getHostName();
    string getLoginCred();
    string getAeName();
    string getAeAppId();
    string getContName();
    string getCseRootAddr();
    string getLocation();
    double getSecondsToDelay();
    int getRuntime();
    bool getSaveConfig();

  private:

    ValidityCheck vc;     // Functions for validity checking on parameters.
    bool successful;      // If parsing was successful.
    string hostName;
    string loginCred;
    string aeName;
    string aeAppId;
    string contName;
    string cseRootAddr;
    string location;
    double secondsToDelay;
    int runtime;
    bool saveConfig;

    void parseFlags(char ** argv, int argc);
    void setDefaults();

};

#endif
