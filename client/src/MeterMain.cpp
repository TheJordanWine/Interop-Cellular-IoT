/**
 * Meter Main Application File:  MeterMain.cpp
 *
 * This file is the main source code file for the client application.
 * It represents the Utility Meter reader and uses various classes to
 * store data.  It utilizes the OneM2M open standards to communicate with
 * the server.
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
#include <algorithm>
#include <iostream>
#include <string>
#include <string.h>
#include <sstream>
#include <fstream>
#include <ctime>
#include "onem2m.hxx"
#include "UtilityMeter.h"

using namespace std;

/**
  * Function declarations.
  */
bool isValidIP(const char x[]);
bool isValidCred(const char x[]);
bool isValidInt(const char x[]);
bool isValidName(const char x[]);
bool isValidPath(const char x[]);
bool writeConfig();
bool readConfig();
onem2m::onem2mResponseStatusCode callbackNotification(
  string h,
  string &from,
  onem2m::notification *n);

// Global Function Variables

string hostName;
string loginCred;
string aeName;
string aeAppId;
string contName;
string location;
string cseRootAddr;
double secondsToDelay;
int runtime;

/**
  * Main function, program entry point.
  */
int main (int argc, char* argv[]) {

  // Local Function Variables
  long result;                          // HTTP Result code.
  hostName = "127.0.0.1:8080";   // The IP:Port of OM2M server.
  loginCred = "admin:admin";     // The OM2M Server login credentials.
  aeName = "MY_METER";           // Name of the AE Resource to create.
  aeAppId = "app1";              // Name of the AE App Id. Mandatory.
  contName = "DATA";             // Data Container Name.
  location = "Home";             // Location of Utility Meter
  bool saveConfig = false;
  ::xml_schema::integer respObjType;    // The response data from server.
  cseRootAddr = "/in-cse/in-name";            // SP-Relative address.
  std::unique_ptr< ::xml_schema::type > respObj;     // The result code from server.
  UtilityMeter um;                      // Construct our UtilityMeter object.
  int meterValue;                       // Represents Utility Meter Value.
  string meterValueStr;                 // Utility Meter Value as a string.
  double secondsPassed;
  secondsToDelay = 10;           // Seconds between meter-value updates
  int count = 0;                        // Test value counter
  runtime = 2;                      // Runtime value in minutes. Default to 2 minutes
  int countCalc = 13;                   // To be calculated using 60 * runtime / secondsToDelay + 1 , Default to 13 for 2 minutes
  double time_counter = 0;              // Timer for simulated data
  clock_t this_time = clock();
  clock_t last_time = this_time;

  // Read Configuration file
  if (readConfig()) {
    cout << "Configuration loaded\n";
  }
  else {
    cout << "Configuration failed to load\n";
  }

  /*
   * Parse for command line flags
   */
  for (int i = 1; i < argc; i++) {
    if (argv[i][0] == '-') { // Check 1st character of arg for flag character
      if (strcmp(argv[i],"-a") == 0) { // aeAppId flag
        cout << "\nCommand line arg passed for AE App Id: ";
        cout << argv[i+1] << endl;
        if (isValidName(argv[i+1])) { // Verify proper format
          aeAppId = argv[i+1];      // set aeAppId to the next argument
        }
        else {
          cout << "Invalid argument for AE App Id - " << argv[i+1]
          << "\n   Exiting...\n";
          return 0;
        }
      }
      else if (strcmp(argv[i],"-c") == 0) { // contName flag
        cout << "\nCommand line arg passed for Container Name: ";
        cout << argv[i+1] << endl;
        if (isValidName(argv[i+1])) { // Verify proper format
          contName = argv[i+1];       // set contName to the next argument
        }
        else {
          cout << "Invalid argument for Container Name - " << argv[i+1]
          << "\n   Exiting...\n";
          return 0;
        }
      }
      else if (strcmp(argv[i],"-d") == 0) { // secondsToDelay flag
        cout << "\nCommand line arg passed for delay in seconds: ";
        cout << argv[i+1] << endl;
        if (isValidInt(argv[i+1])) { // Verify proper format TODO
          secondsToDelay = atoi(argv[i+1]);    // set runtime to the next argument
        }
        else {
          cout << "Invalid argument for delay in seconds - " << argv[i+1]
          << "\n   Exiting...\n";
          return 0;
        }
      }
      else if (strcmp(argv[i],"-h") == 0) { // hostName flag
        cout << "\nCommand line arg passed for OM2M server: ";
        cout << argv[i+1] << endl;
        if (isValidIP(argv[i+1])) { // Verify proper format
          hostName = argv[i+1];    // set hostName to the next argument
        }
        else {
          cout << "Invalid argument for OM2M server - " << argv[i+1]
          << "\n   Exiting...\n";
          return 0;
        }
      }
      else if (strcmp(argv[i],"-l") == 0) { // loginCred flag
        cout << "\nCommand line arg passed for login credentials: ";
        cout << argv[i+1] << endl;
        if (isValidCred(argv[i+1])) { // Verify proper format
          loginCred = argv[i+1];       // set loginCred to the next argument
        }
        else {
          cout << "Invalid argument for login credentials - " << argv[i+1]
          << "\n   Exiting...\n";
          return 0;
        }
      }
      else if (strcmp(argv[i],"-L") == 0) { // location flag * May change naming convention
        cout << "\nCommand line arg passed for location : ";
        cout << argv[i+1] << endl;
        if (isValidName(argv[i+1])) { // Verify proper format
          location = argv[i+1];       // set loginCred to the next argument
        }
        else {
          cout << "Invalid argument for location - " << argv[i+1]
          << "\n   Exiting...\n";
          return 0;
        }
      }
      else if (strcmp(argv[i],"-n") == 0) { // aeName flag
        cout << "\nCommand line arg passed for the AE Resource Name: ";
        cout << argv[i+1] << endl;
        if (isValidName(argv[i+1])) { // Verify proper format
          aeName = argv[i+1];       // set aeName to the next argument
        }
        else {
          cout << "Invalid argument for AE Resource Name - " << argv[i+1]
          << "\n   Exiting...\n";
          return 0;
        }
      }
      else if (strcmp(argv[i],"-r") == 0) { // cseRootAddr flag
        cout << "\nCommand line arg passed for the SP-Relative address: ";
        cout << argv[i+1] << endl;
        if (isValidPath(argv[i+1])) {   // Verify proper format
          cseRootAddr = argv[i+1];      // set cseRootAddr to the next argument
        }
        else {
          cout << "Invalid argument for SP-Relative address - " << argv[i+1]
          << "\n   Exiting...\n";
          return 0;
        }
      }
      else if (strcmp(argv[i],"-t") == 0) { // runtime flag
        cout << "\nCommand line arg passed for run-time in minutes: ";
        cout << argv[i+1] << endl;
        if (isValidInt(argv[i+1])) { // Verify proper format
          runtime = atoi(argv[i+1]);    // set runtime to the next argument
        }
        else {
          cout << "Invalid argument for run-time in minutes - " << argv[i+1]
          << "\n   Exiting...\n";
          return 0;
        }
      }
      else if (strcmp(argv[i],"-s") == 0) { // save configuration flag
        cout << "\nConfiguration will be saved: ";
        saveConfig = true;
      }
      else { // invalid flag
        cout << "Invalid flag  - " << argv[i]
        << "\n   Exiting...\n";
        return 0;
      }
    }
  }

  um.setMeterDescriptor( // Set the Descriptor for the UtilityMeter object.
    "type = Utility_Meter\n"
    "location = " + location + "\n"
    "appIDd = " + aeAppId
  );

  // Calculate count after all arguments have been read
  countCalc = 60 * runtime / secondsToDelay + 1;

  // Save configuration if save flag is preset
  if(saveConfig){
    if(writeConfig()){
      cout << "Configuration successfully saved\n";
    }
    else {
      cout << "Configuration failed to save\n";
    }
  }


  /*
   * First, initialize the OS-IoT library.
   */
  cout << "\nInitializing oneM2M library...";
  onem2m::initialize();
  cout << "Done!\n";

  /*
   * Update the values for pre-set parameters.
   */
  cout << "Updating pre-set parameters...";
  onem2m::setHostName(hostName);    // OM2M server address.
  onem2m::setFrom(loginCred);       // Credentials.
  cout << "Done!\n";

  /*
   * Use the OS-IoT HTTP server implementation to start a HTTP
   * server instance running locally on this client (127.0.0.1)
   * and port number 8082. Use a callback routine callbackNotification.
   */
  onem2m::startHttpServer(vector<string>(), 8082, &callbackNotification);

  /*
   * Create the main MY_METER AE resource object and push to OM2M server.
   * This will be used by the client to send meter values to and to receive
   * commands/instructions from.
   */
  cout << "Creating MY_METER AE resource object...\n";
  auto aeMyMeter = onem2m::AE();
  aeMyMeter.resourceName(aeName);
  aeMyMeter.App_ID(aeAppId);
  aeMyMeter.requestReachability(true); // requestReachability is mandatory for AE.
  auto pl = onem2m::poaList(onem2m::poaList_base(1, "http://127.0.0.1:8082"));
  aeMyMeter.pointOfAccess(pl);
  respObj = onem2m::createResource(cseRootAddr, "5555", aeMyMeter,
    result, respObjType);
  cout << "Result code is:  " << result << "\n";

  /*
   * Retrieve the AE resource.  For testing.
   */
  cout << "\nRetrieving AE resource object...\n";
  respObj = ::onem2m::retrieveResource(cseRootAddr+"/"+aeName,
    "5555", result, respObjType);
  cout << "Result = " << result << "\n";
  cout << "respObjType = " << respObjType << "\n";

  /*
   * Create a container in our AE. This container will store the AE description.
   */
   cout << "\nCreating Descriptor Container...\n";
  auto dsccnt = ::onem2m::container();
  dsccnt.resourceName("DESCRIPTOR");
  respObj = ::onem2m::createResource(cseRootAddr+"/"+aeName,
    "5555", dsccnt, result, respObjType);
  cout << "\nContainer creation result code: " << result << "\n";

  /*
   * Populate the descriptor container with an actual description
   */
  cout << "\nCreating Descriptor Content Instance...\n";
  auto descInst = ::onem2m::contentInstance();
  descInst.contentInfo("application/text");      // Text data.
  descInst.content(um.getMeterDescriptor());
  respObj = ::onem2m::createResource(cseRootAddr+"/"+aeName+"/"+"DESCRIPTOR",
    "5555", descInst, result, respObjType);

  /*
   * Create a Container in our AE. This container will store the meter value.
   */
  cout << "\nCreating DATA Container...\n";
  auto cnt = ::onem2m::container();
  cnt.resourceName(contName);
  respObj = ::onem2m::createResource(cseRootAddr+"/"+aeName,
    "5555", cnt, result, respObjType);
  cout << "\nContainer creation result code: " << result << "\n";

  /*
   * Create a new receiver Container in our AE. The client will subscribe to this
   * container for instructions from the webapp.
   */
  cout << "\nCreating receiver PING_METER Container...\n";
  auto cntRcv = ::onem2m::container();
  cntRcv.resourceName("PING_METER");
  respObj = ::onem2m::createResource(cseRootAddr+"/"+aeName,
    "5555", cntRcv, result, respObjType);
  cout << "\nContainer creation result code: " << result << "\n";

  /*
   * Create a subscription to the OM2M server's PING_METER Container
   * that is under the MY_METER AE.
   */
  cout << "\nSubscribing to PING_METER container on OM2M server...\n";
  auto ss = ::onem2m::subscription();
  auto uri = ::onem2m::listOfURIs();
  ss.resourceName("SUB_CPP_CLIENT");   // The name of the subscription
  uri.push_back(cseRootAddr+"/"+aeName);
  ss.notificationURI(uri);
  ss.notificationContentType(1);  // Receive updates on any resource change.
  respObj = ::onem2m::createResource(cseRootAddr+"/"+aeName+"/"+"PING_METER",
    "5600", ss, result, respObjType);
  cout << "\nSubscription result code: " << result << "\n";

  /*
   * Write simulated utility meter value data to Content Instance in the Container
   */
  cout << "\nCreating Content Instance...\n";
  auto contInst = ::onem2m::contentInstance();
  contInst.contentInfo("application/text");      // Text data.
  meterValue = um.getRandomValue();              // Get a simulated value from meter.
  meterValueStr = "{\"kWH\": " + to_string(meterValue) + "}";   // Convert int to Str.
  contInst.content(meterValueStr);               // Write simulated value.
  respObj = ::onem2m::createResource(cseRootAddr+"/"+aeName+"/"+contName,
    "5555", contInst, result, respObjType);

  /*
   * Get the Content Instance resource. For testing.
   */
  cout << "\nRetrieving Content Instance resource object...\n";
  respObj = ::onem2m::retrieveResource(cseRootAddr+"/"+aeName+"/"+contName,
    "5555", result, respObjType);
  cout << "Result = " << result << "\n";
  cout << "respObjType = " << respObjType << "\n";

  /*
   * Update and display the meter-value every 10 seconds for webapp
   * testing (based on secondsToDelay).
   */
  cout << "Meter values will now update every " << secondsToDelay;
  cout << " seconds for " << runtime;
  cout << " minutes...\n";

    while(count < countCalc){
        this_time = clock();
        time_counter += (double)(this_time - last_time);
        last_time = this_time;

        if(time_counter > (double)(secondsToDelay * CLOCKS_PER_SEC))
        {
            time_counter -= (double)(secondsToDelay * CLOCKS_PER_SEC);

            um.updateMeterValueRand(); //update meterValue with random number.
            meterValue = um.getMeterValue();
            cout << "meter-value: " << meterValue << endl;
            // create a new contentInstance
            contInst = ::onem2m::contentInstance();
            contInst.contentInfo("application/text");      // Text data.
            meterValueStr = "{\"kWH\": " + to_string(meterValue) + "}"; // Convert int to Str.
            contInst.content(meterValueStr);               // Write simulated value.
            respObj = ::onem2m::createResource(cseRootAddr+"/"+aeName+"/"+contName,
              "5555", contInst, result, respObjType);

            count++;
        }
    }

  /*
   * Close the OS-IoT library and exit.
   */
  ::onem2m::terminate();
  cout << "Finished!\n";

  return 1;

} // End of main




/**
  * This function checks to ensure that the provided char
  * array is in a valid IP:Port-number format.  For example,
  * 127.0.0.1:8080 is a valid IP:Port-number formatted char
  * array.
  *
  * @param The input char array to validate.
  * @return Boolean indicating whether input is valid or not.
  */
bool isValidIP(const char x[]) {

  // Initialize the result boolean to true.
  bool result = true;

  int c1 = 0;     // Count number of "." characters.
  int c2 = 0;     // Count number of ":" characters.

  // Iterate through the input char array.
  int i = 0;
  while (x[i] != '\0') {

    // If the character matches "." then increment the relevant counter.
    if (x[i] == '.') {
      c1++;
    }

    // If the character matches ":" then increment the relevant counter.
    else if (x[i] == ':') {
      c2++;
    }

    i++;
  } // End of while loop.

  // If "." characters do not equal 3, result is false.
  if (c1 != 3) {
    result = false;
  }
  // If ":" characters do not equal 1, result is false.
  else if (c2 != 1) {
    result = false;
  }

  // Return the result back.
  return result;

} // End of function isValidIP.


/**
  * This function checks to ensure that the provided char
  * array is in a valid Username:Password format.  For example,
  * admin:admin is a valid Username:Password formatted char
  * array.
  *
  * @param The input char array to validate.
  * @return Boolean indicating whether input is valid or not.
  */
  bool isValidCred(const char x[]) {

    int c1 = 0;     // Count number of ":" characters.

    // Iterate through the input char array.
    int i = 0;
    while (x[i] != '\0') {

      // If the character matches ":" then increment the relevant counter.
      if (x[i] == ':') {
        if (i == 0) { // Username cannot begin with ':', return false if so.
          return false;
        }
        else if (x[i+1] == '\0') { // Check for the last character, return false if character is ':'
          return false;
        }
        c1++;
      }

      i++;
    } // End of while loop.
    //

    if (c1 != 1) { // Return false if there is more than one ':' character
      return false;
    }
    else { // All previous checks have passed, return true
      return true;
    }

  } // End of function isValidCred.


  /**
    * This function checks to ensure that the provided char
    * array is in an integer format.
    *
    * @param The input char array to validate.
    * @return Boolean indicating whether input is valid or not.
    */
  bool isValidInt(const char x[]) {
    // Iterate through the input char array.
    int i = 0;
    while (x[i] != '\0') {
      if (!isdigit(x[i])) { // Return false if not valid integer character
        return false;
      }
      i++;
    }
    return true; // Argument is an integer
  } // End of function isValidInt.

  /**
    * This function checks to ensure that the provided char
    * array is in a name format. This includes alphanumberic characters, '-' and '_'
    *
    * @param The input char array to validate.
    * @return Boolean indicating whether input is valid or not.
    */
  bool isValidName( const char x[]) {
    // Iterate through the input char array.
    int i = 0;
    while (x[i] != '\0') {
      if ( isalnum(x[i]) == 0  && x[i] != '_' && x[i] != '-' ) { // Return false if not a valid alphanumberic, '-' or '_'  character
        return false;
      }
      i++;
    }
    return true; // Argument is an integer
  } // End of function isValidName.

  /**
    * This function checks to ensure that the provided char
    * array is in a file path format beginning with /.
    *
    * @param The input char array to validate.
    * @return Boolean indicating whether input is valid or not.
    */
  bool isValidPath( const char x[] ) {
    int i = 0;
    while (x[i] != '\0') { // get last character of path
      i++;
    }
      return x[0] == '/' && x[i-1] != '/'; // Return true if path begins with / and does not end with /.
  } // End of function isValidPath.

  /**
    * This function reads the configuration file to load the last
    * saved configuration.
    *
    * @return Boolean indicating whether loading of values was successful.
    */
  bool readConfig(){
    string line;
    string key;
    string value;
    int i;
    int colonLocation;
    ifstream configFile ("settings.config"); // Open configuration file
    if ( configFile.is_open() ) { // File successfully opened
      while ( getline (configFile,line) ){ // Get each line of file
        i=0;
        while (line[i] != ':') { // Parse characters of line for ':'
          i++;
        }
        key = line.substr(0,i); // Get characters before ':' character
        cout << "Reading 'key, value' pair: " << key;
        colonLocation = i; // Save location of colon in line
        while (line[i] != '\0') { // parse key
          i++;
        }

        value = line.substr(colonLocation + 1, i); // Get characters  after ':' character
        cout << ", " << value << "\n";

        // check key against options
        if (strcmp(key.c_str(),"aeAppId") == 0) { // aeAppId
          if (isValidName(value.c_str())) { // check for valid name
            aeAppId = value;
          }
          else {
            cout << "Invalid value for aeAppId: " << value << "\n";
            return false;
          }
        }
        else if (strcmp(key.c_str(),"contName") == 0) { // contName
          if (isValidName(value.c_str())) { // check for valid name
            contName = value;
          }
          else {
            cout << "Invalid value for contName: " << value << "\n";
            return false;
          }
        }
        else if (strcmp(key.c_str(),"secondsToDelay") == 0) { // secondsToDelay
          if (isValidInt(value.c_str())) { // Verify proper format
            secondsToDelay = atoi(value.c_str());
          }
          else {
            cout << "Invalid value for secondsToDelay: " << value << "\n";
            return false;
          }
        }
        else if (strcmp(key.c_str(),"hostName") == 0) { // hostName
          if (isValidIP(value.c_str())) { // Verify proper format
            hostName = value;
          }
          else {
            cout << "Invalid value for hostName: " << value << "\n";
            return false;
          }
        }
        else if (strcmp(key.c_str(),"loginCred") == 0) { // loginCred
          if (isValidCred(value.c_str())) { // Verify proper format
            loginCred = value;
          }
          else {
            cout << "Invalid value for loginCred: " << value << "\n";
            return false;
          }
        }
        else if (strcmp(key.c_str(),"location") == 0) { // location
          if (isValidName(value.c_str())) { // check for valid name
            location = value;
          }
          else {
            cout << "Invalid value for location: " << value << "\n";
            return false;
          }
        }
        else if (strcmp(key.c_str(),"aeName") == 0) { // aeName
          if (isValidName(value.c_str())) { // check for valid name
            aeName = value;
          }
          else {
            cout << "Invalid value for aeName: " << value << "\n";
            return false;
          }
        }
        else if (strcmp(key.c_str(),"cseRootAddr") == 0) { // cseRootAddr
          if (isValidPath(value.c_str())) {   // Verify proper format
            cseRootAddr = value;
          }
          else {
            cout << "Invalid value for cseRootAddr: " << value << "\n";
            return false;
          }
        }
        else if (strcmp(key.c_str(),"runtime") == 0) { // runtime
          if (isValidInt(value.c_str())) { // Verify proper format
            runtime = atoi(value.c_str());
          }
          else {
            cout << "Invalid value for runtime: " << value << "\n";
            return false;
          }
        }
        else { // Bad key
          cout << "Unable to locate key: " << key << "\n";
          return false;
        }
      }
      configFile.close(); // Close configuration file
      return true; // return true as nothing failed
    }
    else {
      return false;
    }
  } // End of function readConfig.

  /**
    * This function writes to the configuration file to save the
    * current configuration.
    *
    * @return Boolean indicating whether loading of values was successful.
    */
  bool writeConfig(){ // TODO

  } // End of function writeConfig.


/**
  * This function is the callback routine for onem2m::startHttpServer.
  * It is called when a notification is received and runs as a
  * separate thread from the main.
  */
onem2m::onem2mResponseStatusCode callbackNotification(
  string h,                  // The OM2M 'to' attribute.
  string &from,              // The OM2M 'from' attribute.
  onem2m::notification* n)   // The OM2M notification object contents pointer.
{
  cout << "\nNotification from: " << onem2m::getFrom() << endl;
  return onem2m::rcOK;
}
