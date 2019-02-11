

/**
 * This function gets a random integer value between 0 and the
 * value provided.
 *
 * This function makes use of the existing rand() function which
 * generates a number form 0 to at least 32767. In order to achieve
 * generating a number from 0 to 1000, we have to take the modulus
 * of the resulting random number; the modulus value would be the
 * desired maximum. Then add 1 to the result.
 *
 * @return result The random value between 0 and 1000 that simulates
 * a Utility Meter value.
 */

// Imports
#include <cstdlib>
#include <ctime>
#include "RandomMeterValue.h"
using namespace std;

RandomMeterValue::RandomMeterValue(){
    //TODO set this up so it keeps a static timer value on initialization
    srand(time(NULL));  // Invoke srand() only once, for random values.
    this_time = clock(); // Start the timer
    last_time = this_time; // Save time for future comparisons
    maxRandomValue = 100; // Set a max random value amount
    runningValue = 0; //set the initial value for tracking an on-going value
}

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

int RandomMeterValue::getRunningValue(){
    // calculate random numbers for the full time passed
    for(int i = 0; i < this_time/last_time; i++){
        runningValue += getRandomValue(maxRandomValue);
    }

    return runningValue;
}

void RandomMeterValue::setMaxValue(int newMax){
    maxRandomValue = newMax;
}
