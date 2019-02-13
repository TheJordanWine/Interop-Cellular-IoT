/**
 * Web application file:  app.js
 *
 * This is a web app that allows the user to interface with a oneM2M
 * standards IoT application.  It launches the web application which
 * users will utilize.
 *
 * Instructions:
 *     See docs/webapp.md for the instructions on how to run this
 *         web application.
 *
 * @author Team 6
 *
 * @version 1.0
 */

var express = require('express');
var xmlparser = require('express-xml-bodyparser');
var parseString = require('xml2js').parseString;
var path = require('path');
var fs = require('fs');
var request = require('request');
var session = require('express-session');
var bodyParser = require('body-parser');
const Onem2m = require("./onem2m");
var User = require('./modals/modal');
var mongoose = require('mongoose');

mongoose.connect('mongodb://localhost:27017/admin');
var db = mongoose.connection;
//handle mongo error
db.on('error', console.error.bind(console, 'connection error:'));
db.once('open', function () {
  console.log('we are connected!');
});


let serverOptions = {
    // Listen for client data on the following IP:PORT
    LISTEN_PORT: process.env.npm_config_port || 3000,
    LISTEN_ADDR: process.env.npm_config_addr || "127.0.0.1",
    // IP of OM2M server
    ONE_M2M_HOST: process.env.npm_config_m2mhost || "127.0.0.1",
    // PORT to access OM2M server
    ONE_M2M_PORT: process.env.npm_config_m2mport || 8443,
    AE_NAMES: [],
    IS_HTTPS: process.env.npm_config_ishttps || false
}


var saveDataToJSON = function(ae,ct,incomingData) {
    var dataFolder = ae;

    //Check if the folder is created, if not create it
    //Using sync since async exists is deprecated https://stackoverflow.com/questions/4482686/check-synchronously-if-file-directory-exists-in-node-js/4482701
    if (!fs.existsSync(dataFolder)){
        fs.mkdirSync(dataFolder);
    }

    //Full path to the JSON data file
    var dataFile = path.resolve(__dirname + '/' + dataFolder + '/data.json');

    //Append the data if file exists
    if(fs.existsSync(dataFile)) {
        fs.readFile(dataFile, {encoding: 'utf8'}, function(err, data) {
            if(err) {
                console.log(err);
            }
            var dataJSON = JSON.parse(data);
            var newObj = {
                date: ct,
                data: incomingData
            };
            dataJSON.push(newObj);
            fs.writeFile(dataFile, JSON.stringify(dataJSON, null, 2), {encoding: 'utf8'}, function(err2, data2) {
                if(err2) {
                    console.log(err2);
                }else {
                    console.log('JSON updated in ' + dataFile);
                }
            });
        });
    }else { //brand new file lets make it count!
        var dataJSON = [];
        var newObj = {
            date: ct,
            data: incomingData
        };
        dataJSON.push(newObj);
        fs.writeFile(dataFile, JSON.stringify(dataJSON, null, 2), {encoding: 'utf8'}, function(err2, data2) {
            if(err2) {
                console.log(err2);
            }else {
                console.log('JSON updated in ' + dataFile);
            }
        });
    }
};

function isAuthenticatedCustomMiddleware(req, res, next) {
    var loggedIn = typeof req.session !== 'undefined' ? typeof req.session.isAuth !== 'undefined' ? true : false : false;
    if (loggedIn) {
        return next();
    }
    res.redirect('/login');
}

function subscribeToServer(aeName, subscriptionOpts) {
    var onem2mOptions;
    if(typeof subscriptionOpts !== 'undefined') {
        onem2mOptions = subscriptionOpts;
    }else {
        onem2mOptions = {
            port: serverOptions.ONE_M2M_PORT,
            host: serverOptions.ONE_M2M_HOST,
            listenAddress : serverOptions.LISTEN_ADDR,
            listenPort : serverOptions.LISTEN_PORT,
            aeName : aeName,
            https : serverOptions.IS_HTTPS,
            listenRoute : "/monitor"
        }
    }
     
    
    var onem2m = new Onem2m(onem2mOptions);
    var PendingSub = onem2m.createAE()
        .then( () => { return onem2m.createDataContainer() })
        .then( () => { return onem2m.deleteSubscription() })
        .then( () => { return onem2m.sendSubscription() })
        .catch( (err) => {console.log("Encountered error: " + err.toString())});
    
    return PendingSub;
}





var app = express();
    app.set('view engine', 'pug');
    app.use(express.static(__dirname + '/public'));
    app.use(session({
        secret: 'SER401_SECRET',
        resave: false,
        saveUninitialized: true
    }));
    app.use(bodyParser.urlencoded({extended: true }));
    app.use(xmlparser());



/**
 * Routes to specfic pages
 */
require('./routes/index')(app, isAuthenticatedCustomMiddleware,serverOptions);

require('./routes/login')(app);

require('./routes/logout')(app);

require('./routes/data')(app,isAuthenticatedCustomMiddleware);


/**
 * Routes to API functionality
 */
require('./routes/api/getResource')(app, isAuthenticatedCustomMiddleware, serverOptions);

require('./routes/api/postDelete')(app, serverOptions);

require('./routes/api/postSubscribe')(app,subscribeToServer,serverOptions);

require('./routes/api/status')(app, isAuthenticatedCustomMiddleware, serverOptions);

require('./routes/api/monitor')(app, saveDataToJSON);

app.post('/testaccount', function (req, res) {
    if (req.body.username &&
        req.body.password) {
        var userData = {
            username: req.body.username,
            password: req.body.password,
        }
        //use schema.create to insert data into the db
        User.create(userData, function (err, user) {
            if (err) {
                res.json(err);
            } else {
                res.json({
                    message: "created.."
                })
            }
        });
    }
});



app.listen(serverOptions.LISTEN_PORT, function() {
    console.log('Listening on port: ' + serverOptions.LISTEN_PORT);
    console.log('Head over to ' + serverOptions.LISTEN_ADDR + ':' + serverOptions.LISTEN_PORT);
});



