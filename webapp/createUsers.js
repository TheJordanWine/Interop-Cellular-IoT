var MongoClient = require('mongodb').MongoClient

//
//  config
//

var mongoPort = '27017'
var mongoHost = 'localhost'

var dbName = 'cellulariot'
var userName = 'admin'
var userPassword = 'admin'

//
//  start
//

MongoClient.connect('mongodb://' + mongoHost + ':' + mongoPort + '/' + dbName,
    function (err, db) {

        if (err) {
            return console.log('Error: could not connect to mongodb')
        }


        // Authenticate using the newly added user
        db.authenticate(userName, userPassword, function (err, result) {

            if (err) {
                return console.log('Error: could authenticate with created user')
            }

            console.log('Ok')
            db.close()
        })
    })