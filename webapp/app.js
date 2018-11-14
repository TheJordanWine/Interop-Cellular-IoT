/**
 * Version: 1.0
 * Description: This is a web app that allows a user to interface with a oneM2M
 * complient IoT application.
 */
var express = require('express');
// We'll use request to be able to send post requests to the oneM2M server
var request = require('request');
const LISTEN_PORT = 3000;
const LISTEN_ADDR = "192.168.0.80";
const AE_NAME = "MY_SENSOR";





var app = express();
    app.set('view engine', 'pug');

/**
 * signifys a get request for the inital path of the site
 */
app.get('/', function(req, res) {
    //res is the response object
    //render will take string and search for a pug file in views/ and will render it out
    res.render('index');
});

/**
 * This is where we'll expect the oneM2M server to post info to.
 */
app.post('/monitor', function(req, res) {
    //res is the response object
    //render will take string and search for a pug file in views/ and will render it out
    console.log(req.body);
});

app.listen(LISTEN_PORT, function() {
    console.log('Listening on port 3000...');
    console.log('Head over to localhost:3000');
});


var oneM2MHost = "192.168.0.50";
var oneM2MPort = 8080;
var xmlSubscription = '<m2m:sub xmlns:m2m="http://www.onem2m.org/xml/protocols" rn="SUB_'
xmlSubscription +=AE_NAME +'"><nu>http://';
xmlSubscription += LISTEN_ADDR + ":" + LISTEN_PORT;
xmlSubscription += '/monitor</nu><nct>2</nct></m2m:sub>';
request({
    url: "http://" + oneM2MHost + ':' + oneM2MPort + '/~/in-cse/in-name/' + AE_NAME + '/DATA',
    method: "POST",
    headers: {
        "X-M2M-Origin": "admin:admin",
        "Content-Type": "application/xml;ty=23"
    },
    body: xmlSubscription
}, function (error, response, body){
    console.log(response.statusCode);
    console.log(response.headers);
    console.log("Got from oneM2M server: " + body);  
});
