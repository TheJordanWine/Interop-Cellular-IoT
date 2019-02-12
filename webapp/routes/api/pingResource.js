var path = require('path');
var fs = require('fs');
var request = require('request');
module.exports = function (app, isAuthenticatedCustomMiddleware, serverOpt) {

    app.post('/api/ping', function (req, res) {
        /**
         * Will trigger bidirectional call to Client
         */

        let isHttps = req.body.ishttps,
            om2mhost = req.body.om2mhost,
            om2mport = req.body.om2mport,
            resourceName = req.body.resourceName;
        var PING_METER_CONTAINER = `${isHttps == 'true' ? 'https' : 'http'}://${om2mhost}:${om2mport}/~/in-cse/in-name/${resourceName}/PING_METER`;

        request.post(PING_METER_CONTAINER, {
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
                    // throw new Error("Unable to post data. ");
                    res.json({
                        message: "Error unable to ping meter"
                    });
                }else {
                    res.json({
                        message: "Success! pinged meter successfully"
                    });
                }
            }
        );
    });
};