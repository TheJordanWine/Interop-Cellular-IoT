
#include <iostream>
#include "onem2m.hxx"

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
  ::xml_schema::integer respObjType;    // The response data from server.
  string cseRootAddr = "/in-cse/in-name";            // SP-Relative address.
  std::unique_ptr< ::xml_schema::type > respObj;     // The result code from server.

  // First, initialize the library.
  cout << "\nInitializing oneM2M library...";
  onem2m::initialize();
  cout << "Done!\n";

  // Update the values for pre-set parameters.
  cout << "Updating pre-set parameters...";
  onem2m::setHostName(hostName);    // OM2M server address.
  onem2m::setFrom(loginCred);           // Credentials.
  cout << "Done!\n";

  // Create the MY_METER AE resource object and push to OM2M server.
  cout << "Creating MY_METER AE resource object...\n";
  auto aeMyMeter = onem2m::AE();
  aeMyMeter.resourceName(aeName);
  aeMyMeter.App_ID(aeAppId);
  aeMyMeter.requestReachability(false); // requestReachability is mandatory for AE.
  respObj = onem2m::createResource(cseRootAddr, "5555", aeMyMeter, result, respObjType);
  cout << "Result code is:  " << result << "\n";

  // Get the resource
  cout << "\nRetrieving AE resource object...\n";
  respObj = ::onem2m::retrieveResource("/in-cse/in-name/MY_METER", "5556", result, respObjType);
  cout << "Result = " << result << "\n";
  cout << "respObjType = " << respObjType << "\n";

  // Close the library.
  ::onem2m::terminate();
  cout << "Finished!\n";

  // Exit
  return 1;

} // End of main
