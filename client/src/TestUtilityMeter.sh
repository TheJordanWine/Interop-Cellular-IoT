#
# TestUtilityMeter.sh
#
# This file is used to build/compile the TestUtilityMeter.cpp test
# program.  The TestUtilityMeter.cpp program tests the Implementation
# of the UtilityMeter interface.  This shell script will build the
# binary executable for TestUtilityMeter.cpp by compiling it using g++.
#
# Dependencies:
#   1) This .sh file must be in the same directory as TestUtilityMeter.cpp,
#      UtilityMeter.cpp, and UtilityMeter.h.
#   2) The g++ compiler
#
# Instructions:
#   1) Ensure dependencies are met.
#   2) Execute with "sh TestUtilityMeter.sh". This creates a "test" binary
#      executable in the current directory.
#   3) Execute the compiled binary:
#      ./test
#
# @author Team 6
#
# @version 1.0
#

g++ -o test UtilityMeter.cpp TestUtilityMeter.cpp
