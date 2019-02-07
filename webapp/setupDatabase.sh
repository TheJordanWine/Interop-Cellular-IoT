#!/bin/bash

# Exit if any errors occur
set -e

# Start database
echo "Starting database"
mongod --port 27017 >/dev/null 2>&1 &
sleep 2 # Wait just a bit

# Connect and add user
echo "Creating credentials"
mongo --port 27017 --eval 'db.createUser({user: "myUserAdmin", pwd: "abc123",roles: [ { role: "userAdminAnyDatabase", db: "admin" }, "readWriteAnyDatabase" ]})' admin

# Restart database
echo "Restarting mongodb"
mongod --auth --port 27017  >/dev/null 2>&1 &

echo "DONE"
exit 0