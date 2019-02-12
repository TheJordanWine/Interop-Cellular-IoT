#
# Client Build Script:  ClientBuild_C.sh
#
# This script is used to build/compile the MeterMain.c program.  The
# MeterMain program is the main client side program that runs on the
# resource-constrained device and simulates a Utility Meter Reader
# using the OS-IoT library version for C.
#
# Dependencies:
#   1) This .sh file must be in the client_c/src directory
#   2) The "src" sub-folder is required to contain MeterMain.c.
#   3) The gcc compiler.
#   4) The "git" program in order to install the OS-IoT library
#      as a sub-module in the sub-folder "lib/atis-os-iot-bg96".
#   5) Execute with Sudo or root.
#
# Instructions:
#   1) Ensure dependencies are met.
#   2) Execute with "sh ClientBuild_C.sh". This creates a "main.bin"
#      binary executable in the current directory.
#   3) Execute the compiled binary:
#      ./main.bin
#
# Tested successfully on Debian 9 with all dependencies met.
#
# @author Team 6
#
# @version 1.0
#


#
# Get the OS-IoT library for C BG96 if it doesn't exist.
# This puts the atis-os-iot-bg96 folder in the "lib" subfolder.
#
echo ""
echo "Get the OS-IoT-BG96 library..."
echo ""
if [ -d '../lib/atis-os-iot-bg96' ] ; then
  echo "   Skipping, lib/atis-os-iot-bg96 directory already exists!"
  sleep 2
else
  echo "   Downloading and cloning OS-IoT-BG96 library to lib/atis-os-iot-bg96"
  echo ""
  git clone https://atis.codebasehq.com/atis-os-iot/atis-os-iot-bg96.git ../lib/atis-os-iot-bg96
  sleep 2
  echo "   Finished cloning OS-IoT-BG96 library!"
  echo ""
fi


#
# Copy required .c and .h OS-IoT-BG96 library files to the source directory
# if they don't already exist there.  This is a pre-requisite to using the new
# OS-IoT-BG96 library as per instructions at the link below.
#
# https://os-iot.org/c-apireference/
#
echo ""
echo "Copying required os-iot-bg96 library files to source dir..."
echo ""
cp ../lib/atis-os-iot-bg96/src/demo/onem2m_http.c .
cp ../lib/atis-os-iot-bg96/src/demo/onem2m_http.h .
cp ../lib/atis-os-iot-bg96/src/demo/onem2m_resource.c .
cp ../lib/atis-os-iot-bg96/src/demo/onem2m_resource.h .
cp ../lib/atis-os-iot-bg96/src/demo/onem2m_resource_models.c .
cp ../lib/atis-os-iot-bg96/src/demo/onem2m_resource_models.h .
cp ../lib/atis-os-iot-bg96/src/demo/parson.c .
cp ../lib/atis-os-iot-bg96/src/demo/parson.h .
echo "   Done!"


#
# Compile the client C program now using gcc.
#

# TODO
