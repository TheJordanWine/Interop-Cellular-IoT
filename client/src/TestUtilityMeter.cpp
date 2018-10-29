/**
 * TestUtilityMeter Test File:  TestUtilityMeter.cpp
 *
 * This file is used to test the implementation of the UtilityMeter.cpp
 * code.  It tests the UtilityMeter implementation by building objects
 * of the UtilityMeter type and calling various functions that are defined
 * by UtilityMeter.  The test program compares results from UtilityMeter
 * to see if expected values are returned.  It echos the result of each
 * test.
 *
 * Instructions:
 * Compile this test file along with the UtilityMeter implementation, then
 * execute the binary file using the steps below:
 *    1) Compile:  g++ -o main UtilityMeter.cpp TestUtilityMeter.cpp
 *    2) Execute:  ./main
 *
 * @author Team 6
 *
 * @version 1.0
 */

#include <iostream>
#include <cstdlib>
#include "UtilityMeter.h"

using namespace std;

/**
 * Main function, program entry point.
 */
int main (int argc, char* argv[]) {

  cout << "\nTesting UtilityMeter.cpp Implementation file...\n";

  // Variables in main
  int meterValue;         // The reading value of the meter.
  bool powerStatus;       // The power status of the Utility Meter.
  int setValue;           // The value to set the meter to.

  // Overall test result True = Pass, False = Fail. Initialize to True.
  bool finalResult = true;

  // Declare a pointer to the object first.
  UtilityMeter* u1;

  // Create a UtilityMeter object using default constructor.
  cout << "   * Creating UtilityMeter object.....";
  u1 = new UtilityMeter();
  cout << "Done!\n";

  // The meter value should be zero to start with.
  cout << "   * Starting meter value should be zero:  ";
  meterValue = u1->getMeterValue();
  cout << meterValue << "\n";
  if (meterValue != 0) {
    finalResult = false;
  }

  // The meter should be powered on to start with.
  cout << "   * The Utility Meter should be powered on:  ";
  powerStatus = u1->getPowerStatus();
  cout << powerStatus << "\n";
  if (powerStatus != true) {
    finalResult = false;
  }

  // Set the meter value to a positive value > 0 and read the meter.
  setValue = 555;
  cout << "   * Setting meter to value:  " << setValue << "\n";
  u1->setMeterValue(setValue);
  meterValue = u1->getMeterValue();
  cout << "     Current meter value reading:  " << meterValue << "\n";
  if (meterValue != setValue) {
    finalResult = false;
  }

  // Set the meter value to a negative value.
  // The UtilityMeter is designed to use value 0 in such cases.
  setValue = -555;
  cout << "   * Setting meter to neg value:  " << setValue << "\n";
  u1->setMeterValue(setValue);
  meterValue = u1->getMeterValue();
  cout << "     Current meter value, should be zero:  " << meterValue << "\n";
  if (meterValue != 0) {
    finalResult = false;
  }

  // Set the meter value to 0.
  cout << "   * Testing setting meter to value zero:  ";
  u1->setMeterValue(0);
  meterValue = u1->getMeterValue();
  cout << meterValue << "\n";
  if (meterValue != 0) {
    finalResult = false;
  }

  // Reset the meter value, it should be set to 0.
  cout << "   * Testing the meter value reset function...\n";
  cout << "     Set meter to value:  ";
  u1->setMeterValue(333);
  meterValue = u1->getMeterValue();
  cout << meterValue << "\n";
  cout << "     Initiated Reset...now value is: ";
  u1->resetMeterValue();
  meterValue = u1->getMeterValue();
  cout << meterValue << "\n";
  if (u1->getMeterValue() != 0) {
    finalResult = false;
  }

  // Test the random value generator. The value should be between 0-1000.
  cout << "   * Testing Random meter value generator. Values should be 0-1000: \n";
  int i = 0;            // Iterator.
  int numTests = 10;    // Number of times to Iterate (Num of tests).
  int ranValue;
  while (i < numTests) {
    ranValue = u1->getRandomValue();
    cout << "     Iteration# " << i+1 << " Value: " << ranValue << "\n";
    if ( (ranValue < 0) | (ranValue > 1000) ) {
      finalResult = false;
    }
    i++;
  }

  // Ensure the Meter can be powered off.
  cout << "   * Testing setPowerStatus function to turn the meter off...\n";
  powerStatus = u1->getPowerStatus();
  cout << "     Current power status: " << powerStatus << "\n";
  cout << "     Turning off meter...\n";
  u1->setPowerStatus(false);
  powerStatus = u1->getPowerStatus();
  cout << "     Current power status: " << powerStatus << "\n";
  if (u1->getPowerStatus() != false) {
    finalResult = false;
  }

  // Print the overall result.
  cout << "\nThe final test result:  ";
  if (finalResult != false) {
    cout << "PASSED!";
  }
  else {
    cout << "FAILED!";
  }
  cout << "\n";

  // Return
  return 0;

} // End of Main
