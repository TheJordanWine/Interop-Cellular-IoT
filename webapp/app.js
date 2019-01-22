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
const Onem2m = require("./onem2m");


// Listen for client data on the following IP:PORT
const LISTEN_PORT = process.env.npm_config_port || 3000;
const LISTEN_ADDR = process.env.npm_config_addr || "127.0.0.1";
// One M2M Sever Connection
const ONE_M2M_HOST = process.env.npm_config_m2mhost || "127.0.0.1"; // IP of OM2M server
//const ONE_M2M_PORT = 8080;  // PORT to access OM2M server
const ONE_M2M_PORT = process.env.npm_config_m2mport || 8443;  // PORT to access OM2M server
const AE_NAMES = ["MY_METER"];
const IS_HTTPS = process.env.npm_config_ishttps || false;

function subscribeToServer(aeName) {
    onem2mOptions = {
        port: ONE_M2M_PORT,
        host: ONE_M2M_HOST,
        listenAddress : LISTEN_ADDR,
        listenPort : LISTEN_PORT,
        aeName : aeName,
        https : IS_HTTPS,
        listenRoute : "/monitor"
    }
    var onem2m = new Onem2m(onem2mOptions);
    onem2m.createAE()
    .then( () => { return onem2m.createDataContainer() })
    .then( () => { return onem2m.deleteSubscription() })
    .then( () => { return onem2m.sendSubscription() })
    .catch( (err) => {console.log("Encountered error: " + err.toString())});
}





var app = express();
    app.set('view engine', 'pug');
    app.use(express.static(__dirname + '/public'));

app.use(xmlparser());

/**
 * signifies a get request for the inital path of the site
 */
app.get('/', function(req, res) {
    //res is the response object
    //render will take string and search for a pug file in views/ and will render it out
    res.status(200);
    res.render('index', {
        AENames: AE_NAMES
    });
});

app.get('/data', function(req, res) {
    res.render('data', {
        AEName: req.query.ae
    });
});

app.get('/api/get/:ae', function(req, res) {
    var resourceName = req.params.ae;
    if(AE_NAMES.includes(resourceName)) {
        if(fs.existsSync(resourceName)) {
            var dataFile = path.resolve(__dirname + '/' + resourceName + '/data.json');
            res.sendfile(dataFile);
        }else {
            res.statusCode = 500;
            res.send('null');
            res.end();
        }
    }else {
        res.statusCode = 400;
        res.send('null');
        res.end();
    }
    // res.send("HI");
});
/**
 * Simple ping to the IN-CSE server done server side
 */
app.get('/status', function(req, res) {
    var options = {
        url: `${IS_HTTPS ? 'https' : 'http'}://${ONE_M2M_HOST}:${ONE_M2M_PORT}/~/in-cse`,
        headers: {
          'X-M2M-Origin': 'admin:admin',
          'Accept': 'application/json'
        }
      };
    request(options, function(error, response, body) {
        if (!error && response.statusCode == 200) {
            // res.render('status', {
            //     message: "Server is running!"
            // });
            res.end('true');
        }
        else {
            // res.render('error', {"message" : "Server not running!"});
            res.end('false');
        }
    });
});

/**
 * This is where we'll expect the oneM2M server to post info to.
 */
app.post('/monitor', function(req, res) {
    //res is the response object
    console.log('test');
    var incomingTemp;
    if (req.body['m2m:sgn'].nev) { // First check if we're actually getting a data notification
        parseString(req.body['m2m:sgn'].nev[0].rep[0].con[0], (err, result) => {
            if (!err) {
                incomingTemp = result.obj.int[0]['$'].val;
            } else {
                //get the actual request body content in the form of :  {"temp" : 49}
                var content = req.body['m2m:sgn'].nev[0].rep[0].con[0];
                //get the CT data in the form of : 20181123T112401
                var ct = req.body['m2m:sgn'].nev[0].rep[0].ct[0].match(/(\d{4})(\d{2})(\d{2})(\w)(\d{2})(\d{2})(\d{2})/);
                //Javscript Object Data with toString functionaility
                var creationData = new Date(ct[1], ct[2], ct[3], ct[5], ct[6], ct[7]);
                //Get the application name from the incoming request
                var AEName = req.body['m2m:sgn'].sur[0].match(/(?<=\/)(.*)(?=\/)/)[1].split('/')[2];
                saveDataToJSON(AEName, creationData.toUTCString(), content);

                incomingTemp = JSON.parse(content).temp;
            }

        });
        console.log("Got temperature of: " + incomingTemp);
    }
    res.status(200).send("thanks!");
});

/**
 * Reject anything that is not a post to /monitor
 */
app.all('/monitor', function(req,res) {
    res.status(405);
    res.render('error', {"message" : "Bad method. POST required"});
});



app.listen(LISTEN_PORT, function() {
    console.log('Listening on port: ' + LISTEN_PORT);
    console.log('Head over to ' + LISTEN_ADDR + ':' + LISTEN_PORT);
    AE_NAMES.forEach((aeName) => {
        subscribeToServer(aeName);
    });
});




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
