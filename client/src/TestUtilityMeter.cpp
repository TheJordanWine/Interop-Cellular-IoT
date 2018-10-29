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
 * Compile this test file along with the UtilityMeter implementation, then
 * execute the binary file.
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

  // Overall test result True = Pass, False = Fail. Initialize to True.
  bool finalResult = true;

  // Declare a pointer to the object first.
  UtilityMeter* u1;

  // Create a UtilityMeter object using default constructor.
  u1 = new UtilityMeter();

  // The meter value should be zero to start with.
  if (u1->getMeterValue() != 0) {
    finalResult = false;
  }

  // The meter should be powered on to start with.
  if (u1->getPowerStatus() != true) {
    finalResult = false;
  }

  // Set the meter value to a positive value > 0.
  int setValue = 555;
  u1->setMeterValue(setValue);
  if (u1->getMeterValue() != setValue) {
    finalResult = false;
  }

  // Set the meter value to a negative value.
  // The UtilityMeter is designed to use value 0 in such cases.
  int setNegValue = -555;
  u1->setMeterValue(setNegValue);
  if (u1->getMeterValue() != 0) {
    finalResult = false;
  }

  // Set the meter value to 0.
  u1->setMeterValue(0);
  if (u1->getMeterValue() != 0) {
    finalResult = false;
  }

  // Reset the meter value, it should be set to 0.
  u1->setMeterValue(333);
  u1->resetMeterValue();
  if (u1->getMeterValue() != 0) {
    finalResult = false;
  }

  // Test the random value generator. The value should be between 0-1000.
  int i = 0;            // Iterator.
  int numTests = 10;    // Number of times to Iterate (Num of tests).
  int ranValue;
  while (i < numTests) {
    ranValue = u1->getRandomValue();
    if ( (ranValue < 0) | (ranValue > 1000) ) {
      finalResult = false;
    }
    i++;
  }

  // Ensure the Meter can be powered off.
  u1->setPowerStatus(false);
  if (u1->getPowerStatus() != false) {
    finalResult = false;
  }

  cout << "\nThe final test result:  ";
  cout << finalResult;

  return 0;

} // End of Main
