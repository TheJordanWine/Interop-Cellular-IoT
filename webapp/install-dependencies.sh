#!/bin/bash

# Installation script for dependencies. This is designed to be used by Travis for continous integration
cd webapp

#Install nodejs
sudo apt-get install g++ build-essential -y

# Install webapp dependencies
npm install