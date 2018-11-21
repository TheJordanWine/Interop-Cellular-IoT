/**
 * UtilityMeter Header File:  UtilityMeter.h
 *
 * This file is used to represent the interface for the actual Utility
 * Meter.  It contains private variables that represent a Utility Meter's
 * attributes.  This Utility Meter interface is used to simulate actual
 * Utility Meter Hardware.  It contains public constructors and functions
 * to view/manipulate such attributes.
 *
 * @author Team 6
 *
 * @version 1.0
 */

#ifndef UtilityMeter_H
#define UtilityMeter_H

class UtilityMeter {

  public:

    // Default constructor
    UtilityMeter();

    // Public Functions for this class
    bool getPowerStatus();
    void setPowerStatus(bool inputValue);

    void resetMeterValue();
    void setMeterValue(int inputValue);
    int getMeterValue();
    int getRandomValue();

  private:

    // Private Variables
    int meterValue;
    bool powerStatus;

};

#endif
