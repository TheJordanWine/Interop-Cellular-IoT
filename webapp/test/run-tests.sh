#!/bin/bash

# exit immediately after a failure and be verbose
set -ev

# We assume these are being run from the repository root directory
# This tricks Travis into failing if we're not.
if ls webapp >/dev/null; then
    # Set up environmental variables for testing
    echo "
    export wdio__om2mhost='localhost'
    export wdio__om2mport='8080'
    export wdio__webhost='localhost'
    export wdio__webport='3000'
    export wdio__ishttps='false'
    " > webapp/test/env.sh

    # First start the OM2M server
    echo "Starting OM2M server"
    cd server/IN-CSE
    bash start.sh &
    serverPid=$!
    ps $serverPid && echo "Server running on pid: $serverPid"

    # Next start the webapp
    # Navigate to webapp root dir
    cd ../../webapp
    npm start &
    webAppPid=$1

    # We'll skip installing because we assume Travis has already installed
    # npm install

    # Start Chrome
    google-chrome --no-sandbox --headless --disable-gpu --disable-dev-shm-usage --remote-debugging-port=9222 http://localhost &

    # Run tests
    ln -s /usr/bin/google-chrome-stable /usr/bin/google-chrome
    npm test


    # Stop the server
    kill $serverPid

    # Stop the webapp
    kill $webAppPid

    # Remove env.sh to avoid conflicts
    rm -rf env.sh
else
    echo "Must be executed from the repo root directory"
fi