#!/bin/bash
#
# TestCrypt.sh
#
# This file is used to test the crypt_to_string and string_to_crypt functions
# using cryptTest.cpp.
#
# Instructions:
#   1) make file executable with chmod u+x TestCrypt.sh
#   2) Execute with "sudo ./TestCrypt.sh".
#
# @author Team 6
#
# @version 1.0

g++ -o testCrypt cryptTest.cpp;
./testCrypt;
