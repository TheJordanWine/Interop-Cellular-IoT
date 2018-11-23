
#include <iostream>
#include <string>
#include <sstream>
#include "onem2m.hxx"
#include "UtilityMeter.h"

using namespace std;

/**
 * Main function, program entry point.
 */
int main (int argc, char* argv[]) {

  // Global Function Variables
  long result;                          // HTTP Result code.
  string hostName = "127.0.0.1:8080";   // The IP:Port of OM2M server.
  string loginCred = "admin:admin";     // The OM2M Server login credentials.
  string aeName = "MY_METER";           // Name of the AE Resource to create.
  string aeAppId = "app1";              // Name of the AE App Id. Mandatory.
  string contName = "meter-value";      // Data Container Name.
  ::xml_schema::integer respObjType;    // The response data from server.
  string cseRootAddr = "/in-cse/in-name";            // SP-Relative address.
  std::unique_ptr< ::xml_schema::type > respObj;     // The result code from server.
  UtilityMeter um;                      // Construct our UtilityMeter object.
  int meterValue;                       // Represents Utility Meter Value.
  string meterValueStr;                 // Utility Meter Value as a string.

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
   * Create the main MY_METER AE resource object and push to OM2M server.
   */
  cout << "Creating MY_METER AE resource object...\n";
  auto aeMyMeter = onem2m::AE();
  aeMyMeter.resourceName(aeName);
  aeMyMeter.App_ID(aeAppId);
  aeMyMeter.requestReachability(false); // requestReachability is mandatory for AE.
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
   * Populate the descriptor container with an actual discription
   */
  cout << "\nCreating Descriptor Content Instance...\n";
  auto descInst = ::onem2m::contentInstance();
  descInst.contentInfo("application/text");      // Text data.
  string desctxt= "type = Utility_Meter\n"
    "location = Home\n"
    "appIDd = MY_METER";
   descInst.content(desctxt);
   respObj = ::onem2m::createResource(cseRootAddr+"/"+aeName+"/"+"DESCRIPTOR",
    "5555", descInst, result, respObjType);

  /*
   * Create a Container in our AE. This container will store the meter value.
   */
  cout << "\nCreating Container...\n";
  auto cnt = ::onem2m::container();
  cnt.resourceName(contName);
  respObj = ::onem2m::createResource(cseRootAddr+"/"+aeName,
    "5555", cnt, result, respObjType);
  cout << "\nContainer creation result code: " << result << "\n";

  /*
   * Write simulated utility meter value data to Content Instance in the Container
   */
  cout << "\nCreating Content Instance...\n";
  auto contInst = ::onem2m::contentInstance();
  contInst.contentInfo("application/text");      // Text data.
  meterValue = um.getRandomValue();              // Get a simulated value from meter.
  meterValueStr = to_string(meterValue);         // Convert int to Str.
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
   * Close the OS-IoT library and exit.
   */
  ::onem2m::terminate();
  cout << "Finished!\n";

  return 1;

} // End of main
