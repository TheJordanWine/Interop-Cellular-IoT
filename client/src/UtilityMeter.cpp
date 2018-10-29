/**
 * UtilityMeter Implementation File:  UtilityMeter.cpp
 *
 * This file is used to implement the functions for the actual Utility
 * Meter.  The functions and private variables were already declared
 * in the interface file, so this implementation file is used to define
 * the functions.  It contains the constructor, public functions, and
 * private variables of a UtilityMeter object.
 *
 * @author Team 6
 *
 * @version 1.0
 */

#include <iostream>
#include "UtilityMeter.h"
using namespace std;




/**
 * The Default Constructor for UtilityMeter.  It assigns a default value of
 * "0" to the Meter Value.  Also, a default value of "true" is assigned to the
 * power status.
 */
UtilityMeter::UtilityMeter() {
  powerStatus = true;
  meterValue = 0;
}




/**
 * This function returns the current power status of the Utility Meter
 * object.  A value of "true" indicates that the Utility Meter object is
 * "on" while a value of "false" indicates "off".
 *
 * @return powerStatus The power status of the Utility Meter.
 */
bool UtilityMeter::getPowerStatus() {
  return powerStatus;
}




/**
 * This function sets the power status of the Utility Meter object.
 * A value of "true" indicates that the Utility Meter object is
 * "on" while a value of "false" indicates "off".
 *
 * @param The desired power state to set the Utility Meter to.
 */
UtilityMeter::setPowerStatus(bool inputValue) {
  powerStatus = inputValue;
}




/**
 * This function resets the Meter Value of the Utility Meter object
 * back to a value of "0".
 */
UtilityMeter::resetMeterValue() {
  meterValue = 0;
}




/**
 * This function sets the Meter Value of the Utility Meter object
 * to the specified value.
 *
 * @param inputValue The desired Meter Value for the Utility Meter.
 */
UtilityMeter::setMeterValue(int inputValue) {

  // If inputValue is >0 then use this value.
  if (inputValue > 0) {
    meterValue = inputValue
  }
  // Otherwise, just set the meter value to "0".
  else {
    meterValue = 0
  }

} // End of setMeterValue function




/**
 * This function gets the current Meter Value of the Utility Meter
 * object.
 *
 * @return meterValue The current meter value of the Utility Meter.
 */
int UtilityMeter::getMeterValue() {
  return meterValue;
}
