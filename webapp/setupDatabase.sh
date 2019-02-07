mongod --dbpath=$PWD/database
# db.createUser({
#     "user": "admin",
#     "pwd": "admin",
#     "roles": [
#         {
#             "role": "userAdminAnyDatabase",
#             "db": "admin"
#         }
#     ]
# });