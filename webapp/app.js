/**
 * Version: 1.0
 * Description: This is a web app that allows a user to interface with a oneM2M
 * complient IoT application.
 */
var express = require('express');
var xmlparser = require('express-xml-bodyparser');
var parseString = require('xml2js').parseString;

// We'll use request to be able to send post requests to the oneM2M server
var request = require('request');
const ONE_M2M_HOST = "127.0.0.1";
const ONE_M2M_PORT = 8080;
const LISTEN_PORT = 3000;
const LISTEN_ADDR = "127.0.0.1";
const AE_NAME = "MY_SENSOR";





var app = express();
    app.set('view engine', 'pug');

app.use(xmlparser());

/**
 * signifies a get request for the inital path of the site
 */
app.get('/', function(req, res) {
    //res is the response object
    //render will take string and search for a pug file in views/ and will render it out
    res.status(200);
    res.render('index');
});

/**
 * Simple ping to the IN-CSE server done server side
 */
app.get('/test', function(req, res) {
    var options = {
        url: 'http://127.0.0.1:8080/~/in-cse',
        headers: {
          'X-M2M-Origin': 'admin:admin',
          'Accept': 'application/json'
        }
      };
    request(options, function(error, response, body) {
        if (!error && response.statusCode == 200) {
            res.render('test', {
                response: body
            });
        }
    });
});

/**
 * This is where we'll expect the oneM2M server to post info to.
 */
app.post('/monitor', function(req, res) {
    //res is the response object
    var incomingTemp;
    parseString(req.body['m2m:sgn'].nev[0].rep[0].con[0], (err, result) => {
        incomingTemp = result.obj.int[0]['$'].val;
    });
    
    console.log("Got temperature of: " + incomingTemp);
    res.status(200).send("thanks!");
});

/**
 * Reject anything that is not a post to /monitor
 */
app.all('/monitor', function(req,res) {
    res.status(405);
    res.render('error', {"message" : "Bad method. POST required"});
})

app.listen(LISTEN_PORT, function() {
    console.log('Listening on port 3000...');
    console.log('Head over to localhost:3000');
});




var subscription = {
    "m2m:sub": {
        // Resource Name
        "rn" : "SUB_" + AE_NAME,
        // Notification URI
        "nu" : "http://" + LISTEN_ADDR + ":" + LISTEN_PORT + "/monitor",
        // Notificaation Content Type
        "nct" : 2
    }
}
/**
 * Sends a subscription to the IN-CSE
 */
var sendSubscription = function() {
    request({
        url: "http://" + ONE_M2M_HOST + ':' + ONE_M2M_PORT + '/~/in-cse/in-name/' + AE_NAME + '/DATA',
        method: "POST",
        headers: {
            "X-M2M-Origin": "admin:admin",
            "Content-Type": "application/json;ty=23"
        },
        body: JSON.stringify(subscription)
    }, function (error, response, body){
        if (error) {
            console.log(error);
        } else {
            console.log("prosessing response to subscription");
            console.log(response.statusCode);
            console.log(response.headers);
            console.log("Got from oneM2M server: " + body);  
        }
    });
}

var deleteSubscription = function() {
    request({
        url: "http://" + ONE_M2M_HOST + ':' + ONE_M2M_PORT + '/~/in-cse/in-name/' + AE_NAME + "/DATA/SUB_" + AE_NAME,
        method: "DELETE",
        headers: {
            "X-M2M-Origin": "admin:admin",
            "Accept": "application/json;"
        }
    }, function (error, response, body){
        if (error) {
            console.log(error);
        } else {
            console.log("Subscription was deleted");
        }
    });
}

sendSubscription();
