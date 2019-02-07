# TODO Finish setup of database and user creation.
#mongod --dbpath=$PWD/database
#use admin
#db.createUser(
#  {
#    user: "myUserAdmin",
#    pwd: "abc123",
#    roles: [ { role: "userAdminAnyDatabase", db: "admin" }, "readWriteAnyDatabase" ]
#  }
#)
#mongod --auth --port 27017 --dbpath $PWD/database