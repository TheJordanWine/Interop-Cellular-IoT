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

/**
 * RandomMeterValue Implementation File:  RandomMeterValue.cpp
 *
 */

// Imports
#include <cstdlib>
#include <ctime>
#include <iostream>
#include "RandomMeterValue.h"
using namespace std;

//initialize static variables
int RandomMeterValue::runningValue = 0; //set the initial value for tracking an on-going value
clock_t this_time = clock(); // Start the timer



/**
 * The Default Constructor for RandomMeterValue.  It assigns a default value of
 * "100" to the maxRandomValue.  Also, a default value of "1" or the value of
 * the static variable this_time is assigned to last_time. this_time and 
 * last_time are used to track how much time has passed since the last random
 * value was retrieved.
 */
RandomMeterValue::RandomMeterValue(){
    //TODO set this up so it keeps a static timer value on initialization
    srand(time(NULL));  // Invoke srand() only once, for random values.
    maxRandomValue = 100; // Set a max random value amount
    if(this_time < 1){
        last_time = 1;
    }else{
        last_time = this_time; // Save time for future comparisons
    }
}



/** 
 * This function makes use of the existing rand() function which
 * generates a number form 0 to at least 32767. In order to achieve
 * generating a number from 0 to maxValue, we have to take the modulus
 * of the resulting random number; the modulus value would be the
 * desired maximum. Then add 1 to the result.
 *
 * @param maxValue The highest desired random number.
 * @return result The random value between 0 and the provided maxValue.
 */
int RandomMeterValue::getRandomValue(int maxValue) {

    //verify that the maxValue is positive
    if(maxValue <=0){
        maxValue = 1;
    }else if(maxValue > 32767){
        maxValue = 32767;
    }
    maxRandomValue = maxValue;

  // The desired maximum value for the random number.
  int result = rand() % maxValue + 1;
  return result;
} 




/**
 * This function finds the difference in time since the last update
 * of the runningValue, and updates it in 10 second increments until
 * it matches the current time. This is done to simulate a situation
 * where random values would have accrued the entire time between 
 * function calls.
 * 
 * @return newly updated running value based on time passed since last call
 */
int RandomMeterValue::getRunningValue(){
    int duration = 10000000; //10 sec increments since last value
    this_time = clock(); //get the most updated time
    int loopCount = ((this_time - last_time)/duration); 
    int newVal;
        
    // calculate random numbers for the full time passed
    for(int i = 0; i <= loopCount; i++){
        newVal = getRandomValue(maxRandomValue);
        RandomMeterValue::runningValue += newVal;
    }
    
    //update the last time a value was retrieved
    last_time = this_time;

    return RandomMeterValue::runningValue;
}



/**
 * This function updates the desired maximum random number.
 * 
 * @param newMax The new maximum desired random number value.
 */
void RandomMeterValue::setMaxValue(int newMax){
    maxRandomValue = newMax;
}
