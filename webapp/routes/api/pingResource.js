var path = require('path');
var fs = require('fs');
var request = require('request');
module.exports = function (app, isAuthenticatedCustomMiddleware, serverOpt) {

    app.post('/ping', function(req, res) {
        /**
         * Will trigger bidirectional call to Client
         */
        request.post(`http://127.0.0.1:8080/~/in-cse/in-name/MY_METER/PING_METER`, {
            headers: {
                "content-type": "application/json;ty=4",
                "x-m2m-origin": "admin:admin"
            },
            body: JSON.stringify({
                "m2m:cin": {
                    cnf: "application/text",
                    con: '{"function": "getCurrentMeterReading" }'
                }
            })
            },
            function (err, resp, body) {
                if (err) {
                    throw new Error("Unable to post data. ");
                }
            }
        );    
    });
    
};