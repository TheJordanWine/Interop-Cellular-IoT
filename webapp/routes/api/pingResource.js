var path = require('path');
var fs = require('fs');
var request = require('request');
module.exports = function (app, isAuthenticatedCustomMiddleware, serverOpt) {
    /**
     * Will trigger bidirectional call to Client
     */
    request.post(`${isHttps == "true" ? "https" : "http"}://${om2mhost}:${om2mport}/~/in-cse/in-name/MY_SENSOR/PING_METER`, {
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
};