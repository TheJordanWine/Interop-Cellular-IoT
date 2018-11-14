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
    console.log("prosessing incoming subscription data");
    var incomingTemp;
    parseString(req.body['m2m:sgn'].nev[0].rep[0].con[0], (err, result) => {
        incomingTemp = result.obj.int[0]['$'].val;
    });
    
    console.log("got temperature of: " + incomingTemp);
    res.status(200).send("thanks!");
});

app.all('/monitor', function(req,res) {
    res.status(405).send("Bad method. I need POST");
})

app.listen(LISTEN_PORT, function() {
    console.log('Listening on port 3000...');
    console.log('Head over to localhost:3000');
});


var xmlSubscription = '<m2m:sub xmlns:m2m="http://www.onem2m.org/xml/protocols" rn="SUB_'
xmlSubscription +=AE_NAME +'"><nu>http://';
xmlSubscription += LISTEN_ADDR + ":" + LISTEN_PORT;
xmlSubscription += '/monitor</nu><nct>2</nct></m2m:sub>';
request({
    url: "http://" + ONE_M2M_HOST + ':' + ONE_M2M_PORT + '/~/in-cse/in-name/' + AE_NAME + '/DATA',
    method: "POST",
    headers: {
        "X-M2M-Origin": "admin:admin",
        "Content-Type": "application/xml;ty=23"
    },
    body: xmlSubscription
}, function (error, response, body){
    console.log("prosessing response to subscription");
    console.log(response.statusCode);
    console.log(response.headers);
    console.log("Got from oneM2M server: " + body);  
});
