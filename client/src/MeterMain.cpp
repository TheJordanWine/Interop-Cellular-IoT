
#include <iostream>
#include "onem2m.hxx"

using namespace std;

/**
 * Main function, program entry point.
 */
int main (int argc, char* argv[]) {

  ::onem2m::initialize();
  ::onem2m::setHostName("127.0.0.1:8080");    // OM2M server address.
  string cseRootAddr = "/in-cse/in-name";     // SP-Relative address.
  ::onem2m::setFrom("admin:admin");           // Credentials.

  long result;
  ::xml_schema::integer respObjType;
  std::unique_ptr< ::xml_schema::type > respObj;

  // Get the resource
  respObj = ::onem2m::retrieveResource(cseRootAddr, "5555", result, respObjType);
  cout << "Result = " << result << "\n";
  cout << "respObjType = " << respObjType << "\n";

  // Close connection
  ::onem2m::terminate();

}
