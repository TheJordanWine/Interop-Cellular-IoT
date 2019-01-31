#!/bin/bash
#
# TestMeterMain.sh
#
# This file is used to test MeterMain.cpp.
#
# Instructions:
#   1) make file executable with chmod u+x TestMeterMain.sh
#   2) Execute with "sudo ./TestMeterMain.sh".
#
# @author Team 6
#
# @version 1.0

echo "Testing AE App Id input: appID "
./main -a appID  &
sleep 1
kill $!

echo "Testing  Container Name input: contName "
./main -c contName   &
sleep 1
kill $!

echo "Testing secondsToDelay input: 5 "
./main -d 5  &
sleep 1
kill $!

echo "Testing hostname input: 123.123.123.123:1234"
./main -h 123.123.123.123:1234  &
sleep 1
kill $!

echo "Testing loginCred input: admin:password "
./main -l admin:password  &
sleep 1
kill $!

echo "Testing location input: Tempe "
./main -L Tempe   &
sleep 1
kill $!

echo "Testing aeName input: aeName "
./main -n aeName   &
sleep 1
kill $!

echo "Testing SP-Relative address input: /cseRootAddr "
./main -r /cseRootAddr   &
sleep 1
kill $!

echo "Testing runtime input: 1 "
./main -t 1   &
sleep 1
kill $!

echo "Testing all inputs: "
./main -a appID -c contName -d 5 -h 123.123.123.123:1234 -l admin:password -L Tempe -n aeName -r /cseRootAddr -t 1 &
sleep 1
kill $!

echo "Testing Name Verification: alphanumberic, _ and - allowed"
./main -n asdf.123  &
./main -n as-df_123 &
sleep 1
kill $!

echo "Testing Integer Verification: "
./main -t 1z1  &
./main -t 11   &
sleep 1
kill $!

echo "Testing File Path Verification: must begin with / "
./main -r file/path  &
./main -r /file/path   &
sleep 1
kill $!
