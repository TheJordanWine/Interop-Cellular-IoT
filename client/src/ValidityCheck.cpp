/**
 * Validity Check Implementation File:  ValidityCheck.cpp
 *
 * This file is used to implement the functions for the Validation
 * checks that the MeterMain.cpp will rely on when performing various
 * parsing activities.
 *
 * @author Team 6
 *
 * @version 1.0
 */

//Imports
#include <iostream>
#include <cstdlib>
#include "ValidityCheck.h"
using namespace std;




/**
  * Default constructor
  */
ValidityCheck::ValidityCheck() {
}




/**
  * This function checks to ensure that the provided char
  * array is in a valid IP:Port-number format.  For example,
  * 127.0.0.1:8080 is a valid IP:Port-number formatted char
  * array.
  *
  * @param The input char array to validate.
  * @return Boolean indicating whether input is valid or not.
  */
bool ValidityCheck::isValidIP(const char x[]) {

  // Initialize the result boolean to true.
  bool result = true;

  int c1 = 0;     // Count number of "." characters.
  int c2 = 0;     // Count number of ":" characters.

  // Iterate through the input char array.
  int i = 0;
  while (x[i] != '\0') {

    // If the character matches "." then increment the relevant counter.
    if (x[i] == '.') {
      c1++;
    }

    // If the character matches ":" then increment the relevant counter.
    else if (x[i] == ':') {
      c2++;
    }

    i++;
  } // End of while loop.

  // If "." characters do not equal 3, result is false.
  if (c1 != 3) {
    result = false;
  }
  // If ":" characters do not equal 1, result is false.
  else if (c2 != 1) {
    result = false;
  }

  // Return the result back.
  return result;

} // End of function isValidIP.




/**
  * This function checks to ensure that the provided char
  * array is in a valid Username:Password format.  For example,
  * admin:admin is a valid Username:Password formatted char
  * array.
  *
  * @param The input char array to validate.
  * @return Boolean indicating whether input is valid or not.
  */
bool ValidityCheck::isValidCred(const char x[]) {

    int c1 = 0;     // Count number of ":" characters.

    // Iterate through the input char array.
    int i = 0;
    while (x[i] != '\0') {

      // If the character matches ":" then increment the relevant counter.
      if (x[i] == ':') {
        if (i == 0) { // Username cannot begin with ':', return false if so.
          return false;
        }
        else if (x[i+1] == '\0') { // Check for the last character, return false if character is ':'
          return false;
        }
        c1++;
      }

      i++;
    } // End of while loop.
    //

    if (c1 != 1) { // Return false if there is more than one ':' character
      return false;
    }
    else { // All previous checks have passed, return true
      return true;
    }

} // End of function isValidCred.




/**
  * This function checks to ensure that the provided char
  * array is in an integer format.
  *
  * @param The input char array to validate.
  * @return Boolean indicating whether input is valid or not.
  */
bool ValidityCheck::isValidInt(const char x[]) {
    // Iterate through the input char array.
    int i = 0;
    while (x[i] != '\0') {
      if (!isdigit(x[i])) { // Return false if not valid integer character
        return false;
      }
      i++;
    }
    return true; // Argument is an integer
} // End of function isValidInt.




/**
  * This function checks to ensure that the provided char
  * array is in a name format. This includes alphanumberic characters, '-' and '_'
  *
  * @param The input char array to validate.
  * @return Boolean indicating whether input is valid or not.
  */
bool ValidityCheck::isValidName( const char x[]) {
    // Iterate through the input char array.
    int i = 0;
    while (x[i] != '\0') {
      if ( isalnum(x[i]) == 0  && x[i] != '_' && x[i] != '-' ) { // Return false if not a valid alphanumberic, '-' or '_'  character
        return false;
      }
      i++;
    }
    return true; // Argument is an integer
} // End of function isValidName.




/**
  * This function checks to ensure that the provided char
  * array is in a file path format beginning with /.
  *
  * @param The input char array to validate.
  * @return Boolean indicating whether input is valid or not.
  */
bool ValidityCheck::isValidPath( const char x[] ) {
    int i = 0;
    while (x[i] != '\0') { // get last character of path
      i++;
    }
      return x[0] == '/' && x[i-1] != '/'; // Return true if path begins with / and does not end with /.
} // End of function isValidPath.
