#!/bin/bash

# Install java 8
apt-get install openjdk-8-jdk -y

# Installation script for dependencies. This is designed to be used by Travis for continous integration
cd webapp

# Install webapp dependencies
npm install