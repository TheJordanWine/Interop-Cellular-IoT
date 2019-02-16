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
#include <termios.h>
#include <unistd.h>
#include "onem2m.hxx"
#include "UtilityMeter.h"
#include "ValidityCheck.h"
#include "Arguments.h"

using namespace std;

/**
  * Function declarations.
  */

int getch();
string getpass(const char *prompt, bool show_asterisk);
string string_to_crypt(const char x[]);
string crypt_to_string(const char x[]);
bool writeConfig();
bool readConfig();
onem2m::onem2mResponseStatusCode callbackNotification(
  string h,
  string &from,
  onem2m::notification *n);

// Global Function Variables
string hostName;
string loginCred;
string username;
string password;
string aeName;
string aeAppId;
string contName;
string cseRootAddr;
string location;
double secondsToDelay;
int runtime;
bool saveConfig;
ValidityCheck vc;
UtilityMeter um;       // Construct our UtilityMeter object.
::xml_schema::integer respObjType;    // The response data from server.
std::unique_ptr< ::xml_schema::type > respObj;  // The result code from server.




/**
  * Main function, program entry point.
  */
int main (int argc, char* argv[]) {
  long result;                   // HTTP Result code.
  int meterValue;                // Represents Utility Meter Value.
  string meterValueStr;          // Utility Meter Value as a string.
  double secondsPassed;
  int count = 0;                 // Test value counter
  int countCalc = 13;            // To be calculated using 60 * runtime / secondsToDelay + 1 , Default to 13 for 2 minutes
  double time_counter = 0;       // Timer for simulated data
  clock_t this_time = clock();
  clock_t last_time = this_time;

  // Read Configuration file
  if (readConfig()) {
    cout << "Configuration loaded\n";
  }
  else {
    cout << "Configuration failed to load\n";
  }

  // Process command line args.
  cout << "Processing command line args...\n";
  Arguments a(argv, argc);        // Call constructor for processing args.
  if (!(a.getSuccessful())) {     // Exit if problem parsing flags.
    return 0;
  }
  // Set values
  hostName = a.getHostName();     // The IP:Port of OM2M server.
  loginCred = a.getLoginCred();   // The OM2M Server login credentials.
  aeName = a.getAeName();         // Name of the AE Resource to create.
  aeAppId = a.getAeAppId();       // Name of the AE App Id. Mandatory.
  contName = a.getContName();     // Data Container Name.
  location = a.getLocation();     // Location of Utility Meter
  saveConfig = a.getSaveConfig(); // Whether to save config to file or not.
  cseRootAddr = a.getCseRootAddr();       // SP-Relative address.
  secondsToDelay = a.getSecondsToDelay(); // Seconds between meter-value updates
  runtime = a.getRuntime();       // Runtime value in minutes. Default to 2 minutes

  // Set the Descriptor for the UtilityMeter object.
  um.setMeterDescriptor(
    "type = Utility_Meter\n"
    "location = " + location + "\n"
    "appIDd = " + aeAppId
  );

  // Calculate count after all arguments have been read
  countCalc = 60 * runtime / secondsToDelay + 1;

  // Build login string if password was prompted
  if (promptPass) {
     loginCred = username + ":" + password;
  }
  // Save configuration if save flag is preset
  if(saveConfig){
    if( writeConfig() ){
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
  onem2m::setFrom(username + ":" + password);       // Credentials.
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
  meterValue = um.updateMeterValueRand();        // Get a simulated value from meter.
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

            meterValue = um.updateMeterValueRand(); //update meterValue with random number.
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
    ifstream configFile ("settings.config"); // Open configuration file for reading
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
          if (vc.isValidName(value.c_str())) { // check for valid name
            aeAppId = value;
          }
          else {
            cout << "Invalid value for aeAppId: " << value << "\n";
            return false;
          }
        }
        else if (strcmp(key.c_str(),"contName") == 0) { // contName
          if (vc.isValidName(value.c_str())) { // check for valid name
            contName = value;
          }
          else {
            cout << "Invalid value for contName: " << value << "\n";
            return false;
          }
        }
        else if (strcmp(key.c_str(),"secondsToDelay") == 0) { // secondsToDelay
          if (vc.isValidInt(value.c_str())) { // Verify proper format
            secondsToDelay = atoi(value.c_str());
          }
          else {
            cout << "Invalid value for secondsToDelay: " << value << "\n";
            return false;
          }
        }
        else if (strcmp(key.c_str(),"hostName") == 0) { // hostName
          if (vc.isValidIP(value.c_str())) { // Verify proper format
            hostName = value;
          }
          else {
            cout << "Invalid value for hostName: " << value << "\n";
            return false;
          }
        }
        else if (strcmp(key.c_str(),"username") == 0) { // username
          if (isValidName(value.c_str())) { // Verify proper format
            username = value;
          }
          else {
            cout << "Invalid value for username: " << value << "\n";
            return false;
          }
        }
        else if (strcmp(key.c_str(),"password") == 0) { // password
          if (isValidPass(value.c_str())) { // Verify proper format
            password = crypt_to_string(value.c_str());
          }
          else {
            cout << "Invalid value for password: " << value << "\n";
            return false;
          }
        }
        else if (strcmp(key.c_str(),"location") == 0) { // location
          if (vc.isValidName(value.c_str())) { // check for valid name
            location = value;
          }
          else {
            cout << "Invalid value for location: " << value << "\n";
            return false;
          }
        }
        else if (strcmp(key.c_str(),"aeName") == 0) { // aeName
          if (vc.isValidName(value.c_str())) { // check for valid name
            aeName = value;
          }
          else {
            cout << "Invalid value for aeName: " << value << "\n";
            return false;
          }
        }
        else if (strcmp(key.c_str(),"cseRootAddr") == 0) { // cseRootAddr
          if (vc.isValidPath(value.c_str())) {   // Verify proper format
            cseRootAddr = value;
          }
          else {
            cout << "Invalid value for cseRootAddr: " << value << "\n";
            return false;
          }
        }
        else if (strcmp(key.c_str(),"runtime") == 0) { // runtime
          if (vc.isValidInt(value.c_str())) { // Verify proper format
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
  bool writeConfig(){
      ofstream configFile ( "settings.config" ); // Open configuration for writing
      if ( configFile.is_open() ) { // Check if file is open, then input values
        configFile << "aeAppId:" << aeAppId << "\n";
        configFile << "contName:" << contName << "\n";
        configFile << "secondsToDelay:" << secondsToDelay << "\n";
        configFile << "hostName:" << hostName << "\n";
        configFile << "username:" << username << "\n";
        configFile << "password:" << string_to_crypt(password.c_str()) << "\n";
        configFile << "location:" << location << "\n";
        configFile << "aeName:" << aeName << "\n";
        configFile << "cseRootAddr:" << cseRootAddr << "\n";
        configFile << "runtime:" << runtime << "\n";
        configFile.close(); // close file
        return true;
      }
      else {
        return false;
      }

  } // End of function writeConfig.

  /**
    * This function gets a character from the user while masking input.
    * Used with getpass()
    * Depends on termios package
    * @return character as an integer
    */
  int getch() {
    int ch;
    struct termios t_old, t_new;
    tcgetattr(STDIN_FILENO, &t_old);
    t_new = t_old;
    t_new.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &t_new);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &t_old);
    return ch;
} // End of function getch

/**
  * This function gets a password from the user while hiding input.
  * Uses with getchar()
  * Depends on termios package
  * @param The prompt to be displayed to user.
  * @param Whether asterisks should be displayed in place of password
  * @return password as a string
  */
string getpass(const char *prompt, bool show_asterisk) {
  const char BACKSPACE=127; // integer value of backspace character
  const char RETURN=10; // integer value of return character
  string password;
  unsigned char ch=0;
  cout <<prompt<<endl; // Output password prompt
  while((ch=getch())!=RETURN) { // Get input until return character
       if(ch==BACKSPACE) { // remove last character if backspace received
            if(password.length()!=0) {
              if(show_asterisk) {
                 cout <<"\b \b";
               }
                 password.resize(password.length()-1);
              }
         }
       else {
             password+=ch;
             if(show_asterisk) { // Display asterisk instead of input character
                 cout <<'*';
               }
         }
    }
  cout << endl;
  return password;
} // End of function getpass


/**
  * This function gets a plain text string and converts it into hexadecimal.
  * Each individual character is converted into two hex characters
  * @param plain text string
  * @return string as hexadecimal
  */
  string string_to_crypt(const char x[]) {
    string hexString;
    stringstream sstream;
    int i = 0;
    int hexLength;
    while (x[i] != '\0') {
      sstream << hex << (int)x[i] + i + 1;
      i++;
    }
    hexString = sstream.str();
    hexLength = hexString.length();
    for (int j = 0; j < hexLength; j++) {
      hexString[j] = (int)hexString[j] + 20-j;
    }
    cout << endl;
    return hexString;
  } // End of function string_to_crypt

/**
  * This function gets a hexadecimal string and converts it into plain text.
  * Each individual character is converted into two hex characters
  * @param hexadecimal string of even length
  * @return string as plain text
  */
  string crypt_to_string(const char x[]) {
    string plainString = "";
    char hex[2];
    char hexCharArr[64];
    int i = 0;
    int j = 0;
    while (x[j] != '\0') {
      hexCharArr[j] =  (char)((int)x[j] - 20 + j);
      j++;
    }
    hexCharArr[j] = '\0';
    while (x[i] != '\0') {
      hex[0] = hexCharArr[ i ];
      hex[1] = hexCharArr[ i+1 ];
      plainString = plainString + (char)(stoi(hex,nullptr,16) - i/2 - 1);
      i += 2;
    }
    return plainString;
  } // End of function crypt_to_string

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
  // Function Variables
  int meterReading;
  string meterReadingStr;
  long result;
  int stringCompareResult;   // For comparing strings. 0 means equal.

  // Write to console
  cout << "\nCallback executed - Notification from: " << onem2m::getFrom() << endl;

  // Process verificationRequest and decide to accept it here.
  if (n->verificationRequest().present()) {
    cout << "   Processing verification request..." << endl;

    // TODO: Check "From" field to validate if desired.

    return onem2m::rcOK;
  } // End of verificationRequest processing.

  cout << "Sending meter value to OM2M server...\n";
  // Get the current meter value of the UtilityMeter object.
  meterReading = um.getMeterValue();

  // Convert the meter reading to string.
  meterReadingStr = "{\"kWH\": " + to_string(meterReading) + "}";

  // Create the content instance to write the meter value to
  auto ci = ::onem2m::contentInstance();
  ci.contentInfo("application/text");      // Text data.
  ci.content(meterReadingStr);             // Write meter value.

  // Post the meter value to the DATA container on OM2M server.
  respObj = ::onem2m::createResource(cseRootAddr+"/"+aeName+"/"+"DATA",
    "6000", ci, result, respObjType);
  cout << "   Result = " << result << "\n";

  // Return HTTP OK
  return onem2m::rcOK;

} // End of callbackNotification function

