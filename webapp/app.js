/**
 * Version: 1.0
 * Description: This is a web app that allows a user to interface with a oneM2M
 * complient IoT application.
 */
var express = require('express');
var xmlparser = require('express-xml-bodyparser');
var parseString = require('xml2js').parseString;
var path = require('path');
var fs = require('fs');

// We'll use request to be able to send post requests to the oneM2M server
var request = require('request');
const ONE_M2M_HOST = "127.0.0.1";
const ONE_M2M_PORT = 8080;
const LISTEN_PORT = 3000;
const LISTEN_ADDR = "127.0.0.1";
const AE_NAMES = ["MY_SENSOR" , "MY_METER"];





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
        else {
            res.render('error', {"message" : "Server not running!"});
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
        if (!err) {
            incomingTemp = result.obj.int[0]['$'].val;
        } else {
<<<<<<< HEAD
            var content = req.body['m2m:sgn'].nev[0].rep[0].con[0];
            incomingTemp = JSON.parse(content).temp;
        }
=======
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

>>>>>>> 2d76435825176831ab21ce67dae07b29b41c14c8
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
});

app.listen(LISTEN_PORT, function() {
    console.log('Listening on port 3000...');
    console.log('Head over to localhost:3000');
});




var saveDataToJSON = function(ae,ct,incomingData) {
    var dataFolder = ae + '_data';

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

/**
 * Sends a subscription to the IN-CSE
 */
var sendSubscription = function() {
    AE_NAMES.forEach( (AE_NAME) =>  {
        var subscription = {
            "m2m:sub": {
                // Resource Name
                "rn" : "SUB_" + AE_NAME,
                // Notification URI
                "nu" : "http://" + LISTEN_ADDR + ":" + LISTEN_PORT + "/monitor",
                // Notificaation Content Type
                "nct" : 2
            }
        };   
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
    });
<<<<<<< HEAD
}

var deleteSubscription = function(callback) {
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
            callback();
        }
=======
};


var deleteSubscription = function(callback) {
    AE_NAMES.forEach( (AE_NAME) =>  {
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
                callback();
            }
        });
>>>>>>> 2d76435825176831ab21ce67dae07b29b41c14c8
    });
};


var cleanSubscription = function() {
    deleteSubscription(sendSubscription);
};

<<<<<<< HEAD
var cleanSubscription = function() {
    deleteSubscription(sendSubscription);
}

=======
>>>>>>> 2d76435825176831ab21ce67dae07b29b41c14c8
cleanSubscription();
