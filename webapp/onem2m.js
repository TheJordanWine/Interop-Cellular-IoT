
// We'll use request to be able to send post requests to the oneM2M server
var request = require('request');

module.exports = class Onem2m {
    constructor(options) {
        if (options.https) {
            this.https = true;
        } else {
            this.https = false;
        }
        if (options.host) {
            this.host = options.host;
        } else {
            this.host = "127.0.0.1"
        }
        if (options.port) {
            this.port = options.port;
        } else {
            this.port = 8080;
        }
        if (options.caPath) {
            this.caPath = options.caPath;
        }
        if (options.caInfo) {
            this.caInfo = options.caInfo;
        }
        if (options.subjectANA) {
            this.subjectANA = options.subjectANA;
        }
        if (options.aeName) {
            this.aeName = options.aeName;
        } else {
            this.aeName = "MY_SENSOR";
        }
        if (options.listenAddress) {
            this.listenAddress = options.listenAddress;
        } else {
            this.listenAddress = "127.0.0.1";
        }
        if (options.listenPort) {
            this.listenPort = options.listenPort;
        } else {
            this.listenPort = 3000;
        }
        if (options.listenRoute) {
            this.listenRoute = options.listenRoute;
        } else {
            this.listenRoute = "/monitor";
        }
        if (options.credentials) {
            this.credentials = options.credentials;
        } else {
            this.credentials = "admin:admin";
        }
    }

    
    /**
     * Sends a subscription to the IN-CSE
     */
    sendSubscription() {
        console.log("sending  subscription");
        var subscription = {
            "m2m:sub": {
                // Resource Name
                "rn" : "SUB_" + this.aeName,
                // Notification URI
                "nu" : "http://" + this.listenAddress + ":" + this.listenPort + this.listenRoute,
                // Notificaation Content Type
                "nct" : 2
            }
        };
        var requestContent = {
            url: "http://" + this.host + ':' + this.port + '/~/in-cse/in-name/' + this.aeName + '/DATA',
            method: "POST",
            headers: {
                "X-M2M-Origin": this.credentials,
                "Content-Type": "application/json;ty=23"
            },
            body: JSON.stringify(subscription)
        }
        return new Promise(function(resolve, reject) {
            request(requestContent, function (error, response, body){
                if (error) {
                    reject(error);
                } else {
                    resolve();
                }
            });
        });
    }


    deleteSubscription() {
        console.log("deleting subscription");
        var requestContent = {
            url: "http://" + this.host + ':' + this.port + '/~/in-cse/in-name/' + this.aeName + "/DATA/SUB_" + this.aeName,
            method: "DELETE",
            headers: {
                "X-M2M-Origin": this.credentials,
                "Accept": "application/json;"
            }
        }
        return new Promise(function(resolve, reject) {
            request(requestContent, function (error, response, body){
                if (error) {
                    reject(error);
                } else {
                    resolve();
                }
            });
        });
    };

    createAE() {
        console.log("creating AE");
        var requestBody = {
            "m2m:ae": {
                // Resource Name
                "rn" : this.aeName,
                // Notification URI
                "api" : "app-sensor",
                // Notificaation Content Type
                "lbl" : ["Type/sensor", "Category/temperature", "Location/home"],
                "rr" : "false"
            }
        };   
        var requestContent = {
            url: "http://" + this.host + ':' + this.port + '/~/in-cse',
            method: "POST",
            headers: {
                "X-M2M-Origin": this.credentials,
                "Content-Type": "application/json;ty=2"
            },
            body: JSON.stringify(requestBody)
        }
        return new Promise(function(resolve, reject) {
            request(requestContent, function (error, response, body){
                if (error) {
                    reject(error);
                } else {
                    resolve();
                }
            });
        });
    }

    createDataContainer() {
        console.log("creating container");
        var requestBody = {
            "m2m:cnt": {
                // Resource Name
                "rn" : "DATA",
            }
        };
        var requestContent = {
            url: "http://" + this.host + ':' + this.port + '/~/in-cse/in-name/' + this.aeName,
            method: "POST",
            headers: {
                "X-M2M-Origin": this.credentials,
                "Content-Type": "application/json;ty=3"
            },
            body: JSON.stringify(requestBody)
        }
        return new Promise(function(resolve, reject) {
            request(requestContent, function (error, response, body){
                if (error) {
                    reject(error);
                } else {
                    resolve();
                }
            });
        });
    }

}