#!/bin/bash

# Exit if any errors occur
set -e


# Start database
echo "Starting database"
mongod --port 27017 >/dev/null 2>&1 &
sleep 2 # Wait just a bit

# Connect and add user
echo "Creating credentials"
if mongo --port 27017 --eval 'db.createUser({user: "myUserAdmin", pwd: "abc123",roles: [ { role: "userAdminAnyDatabase", db: "admin" }, "readWriteAnyDatabase" ]})' admin; then
    echo "Successfully added user"
else
    retVal=$?
    if [[ ${retVal} -eq 127 ]]; then
        echo "Success, user already exists"
    else
        echo "Error code: ${retVal}"
        exit ${retVal}
    fi
fi

# Restart database
echo "Restarting mongodb"
mongod --auth --port 27017  >/dev/null 2>&1 &

echo "DONE"
exit 0