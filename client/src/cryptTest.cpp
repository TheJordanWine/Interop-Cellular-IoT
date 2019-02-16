#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <iostream>
#include <string>
#include <string.h>
#include <sstream>
#include <fstream>
#include <ctime>
#include <termios.h>
#include <unistd.h>
#include <math.h>
#include "onem2m.hxx"
#include "UtilityMeter.h"
using namespace std;


string string_to_crypt(const char x[]) {
  string hexString;
  stringstream sstream;
  int charVal;
  int i = 0;
  int hexLength;
  while (x[i] != '\0') {
    sstream << hex << (int)x[i] + i + 1;
    i++;
  }
  hexString = sstream.str();
  hexLength = hexString.length();
  for (int j = 0; j < hexLength; j++) {
    hexString[j] = (int)hexString[j] + 20-j;
  }
  cout << endl;
  return hexString;
} // End of function string_to_crypt

/**
* This function gets a hexadecimal string and converts it into plain text.
* Each individual character is converted into two hex characters
* @param hexadecimal string of even length
* @return string as plain text
*/
string crypt_to_string(const char x[]) {
  string plainString = "";
  char hex[2];
  char hexCharArr[64];
  int i = 0;
  int j = 0;
  while (x[j] != '\0') {
    hexCharArr[j] =  (char)((int)x[j] - 20 + j);
    j++;
  }
  hexCharArr[j] = '\0';
  while (x[i] != '\0') {
    hex[0] = hexCharArr[ i ];
    hex[1] = hexCharArr[ i+1 ];
    plainString = plainString + (char)(stoi(hex,nullptr,16) - i/2 - 1);
    i += 2;
  }
  return plainString;
} // End of function crypt_to_string


int main ()
{
  string password = "MyPassword123!";
  cout << "Plain text: " << password << endl;
  cout << "Encrypted text: " <<string_to_crypt(password.c_str()) << endl;
  cout << "Decrypted text: " << crypt_to_string(string_to_crypt(password.c_str()).c_str() ) << endl;

  password = "zzzzzzzzzzzzzzzzzzzz";
  cout << "Plain text: " << password << endl;
  cout << "Encrypted text: " <<string_to_crypt(password.c_str()) << endl;
  cout << "Decrypted text: " << crypt_to_string(string_to_crypt(password.c_str()).c_str() ) << endl;

  return 0;
}
