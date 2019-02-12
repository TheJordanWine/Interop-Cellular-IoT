#!/bin/bash
#
# ClientBuild.sh
#
# This script is used to build/compile the MeterMain.cpp program.  The
# MeterMain program is the main client side program that runs on the
# resource-constrained device and simulates a Utility Meter Reader.
#
# Dependencies:
#   1) This .sh file must be in the client/src directory
#   2) The "src" sub-folder is required to contain MeterMain.cpp,
#      UtilityMeter.cpp, and UtilityMeter.h.
#   3) The g++ compiler.
#   4) The "lib/xsd/cxx" subfolder must contain the codesynthesis
#      header files.
#   5) The "git" program in order to install the OS-IoT library
#      as a sub-module in the sub-folder "lib/atis-os-iot".
#   6) The programs:  libxerces-c-dev, libcurl4-openssl-dev,
#      libssl1.0-dev.
#   7) Execute with Sudo or root.
#
# Instructions:
#   1) Ensure dependencies are met.
#   2) Execute with "sh ClientBuild.sh". This creates a "main" binary
#      executable in the current directory.
#   3) Execute the compiled binary:
#      ./main
#
# Tested successfully on Debian 9 with all dependencies met.
#
# @author Team 6
#
# @version 1.0
#


#
# Install dependencies
# g++ libxerces-c-dev libcurl4-openssl-dev libssl1.0-dev make git
#
echo ""
echo "Installing Linux package dependencies..."
aptIsInstalled=`which apt-get | wc -l`  # Check if "apt-get" is installed.
insIsInstalled=`which install | wc -l`  # Check if "install" is installed.
if [ $aptIsInstalled > 0 ] && [ $insIsInstalled > 0 ] ; then
  echo "   The apt-get and install packages exist."
  echo "   Now installing Linux package dependencies via apt-get install..."
  apt-get install -qq g++ libxerces-c-dev libcurl4-openssl-dev libssl1.0-dev make git
  echo ""
  echo "   Dependencies were installed."
else
  echo "   Could not install Linux package dependencies"
  echo "   The apt-get and install binaries do not exist..."
  echo ""
fi


#
# Get the OS-IoT library if it doesn't exist.
# This puts the atis-os-iot folder in the "lib" subfolder.
#
echo ""
echo "Get the OS-IoT library..."
echo ""
if [ -d '../lib/atis-os-iot' ] ; then
  echo "   Skipping, lib/atis-os-iot directory already exists!"
  sleep 2
else
  echo "   Downloading and cloning OS-IoT library to lib/atis-os-iot"
  echo ""
  git clone https://atis.codebasehq.com/atis-os-iot/atis-os-iot.git ../lib/atis-os-iot
  sleep 2
  echo "   Done"
fi


#
# The Codesynthesis package headers are stored in the lib/xsd sub-subfolder
# and needs to be copied to /usr/local/include.  Need sudo for this step.
#
echo ""
echo "Copy Codesynthesis package headers..."
if [ -d '../lib/xsd' ] ; then
  cp -r ../lib/xsd /usr/local/include
  sleep 2
  echo "   Done!"
else
  echo "Codesynthesis package doesn't exist in lib!"
  echo "   Pull the latest Interop-Cellular-IoT branch to ensure they exist then re-run"
  echo "   Exiting..."
  exit 0
fi


#
# Dynamically link the OS-IoT library to this project.
#
echo ""
echo "Dynamically linking OS-IoT library to this project..."
if [ -e '../lib/atis-os-iot/libosiot.so' ] ; then
  echo "   Dynamically linked file exists, skipping step..."
else
  make -C ../lib/atis-os-iot libosiotdyn
  sleep 2
  echo "   Done!"
fi


#
# Copy dynamically linked files into src and src/cdt
#
echo ""
echo "Copying dynamically linked files into src and src/cdt..."
echo ""
if [ ! -d cdt ] ; then
  mkdir cdt
fi
cp ../lib/atis-os-iot/libosiot.so .
cp ../lib/atis-os-iot/CDT-*.hxx .
cp ../lib/atis-os-iot/onem2m.hxx .
cp ../lib/atis-os-iot/cdtHeads.hxx .
cp ../lib/atis-os-iot/cdt/*.hxx cdt/
echo "   Done!"


#
# Compile the program now using g++.
#
echo ""
echo "Compiling program with g++..."
g++ -v -o main MeterMain.cpp UtilityMeter.cpp -Wl,-rpath=. -L. -losiot -lssl -lcrypto -lxerces-c -lcurl -lpthread

#
# Done
#
echo ""
echo "Completed Building Utility Meter Client application!"
echo "   To execute:  ./main"
