// We'll use request to be able to send post requests to the oneM2M server
var request = require("request");
var fs = require("fs");
const CA_FILE = "./certstore/serverCertificate.pem";

// Accept self-signed certs for now
process.env["NODE_TLS_REJECT_UNAUTHORIZED"] = 0;

module.exports = class Onem2m {
  constructor(options) {
    // Copy options
    this.options = options;

    // Set defaults for required options
    this.options.host = "127.0.0.1" || options.host;
    this.options.port = 8080 || options.port;
    this.options.aeName = "MY_METER" || options.aeName;
    this.options.listenAddress = "127.0.0.1" || options.listenAddress;
    this.options.listenPort = 3000 || options.listenPort;
    this.options.listenRoute = "/monitor" || options.listenRoute;
    this.options.credentials = "admin:admin" || options.credentials;
  }

  /**
   * Sends a subscription to the IN-CSE
   */
  sendSubscription() {
    console.log("sending  subscription");
    var subscription = {
      "m2m:sub": {
        // Resource Name
        rn: "SUB_" + this.options.aeName,
        // Notification URI
        nu:
          "http://" +
          this.options.listenAddress +
          ":" +
          this.options.listenPort +
          this.options.listenRoute,
        // Notificaation Content Type
        nct: 2
      }
    };
    var protocol;
    if (this.options.https) {
      protocol = "https";
    } else {
      protocol = "http";
    }
    var requestContent = {
      url:
        protocol +
        "://" +
        this.options.host +
        ":" +
        this.options.port +
        "/~/in-cse/in-name/" +
        this.options.aeName +
        "/DATA",
      method: "POST",
      ca: fs.readFileSync(CA_FILE),
      headers: {
        "X-M2M-Origin": this.options.credentials,
        "Content-Type": "application/json;ty=23"
      },
      body: JSON.stringify(subscription)
    };
    return new Promise(function(resolve, reject) {
      request(requestContent, function(error, response, body) {
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
    var protocol;
    if (this.options.https) {
      protocol = "https";
    } else {
      protocol = "http";
    }
    var requestContent = {
      url:
        protocol +
        "://" +
        this.options.host +
        ":" +
        this.options.port +
        "/~/in-cse/in-name/" +
        this.options.aeName +
        "/DATA/SUB_" +
        this.options.aeName,
      method: "DELETE",
      ca: fs.readFileSync(CA_FILE),
      headers: {
        "X-M2M-Origin": this.options.credentials,
        Accept: "application/json;"
      }
    };
    return new Promise(function(resolve, reject) {
      request(requestContent, function(error, response, body) {
        if (error) {
          reject(error);
        } else {
          resolve();
        }
      });
    });
  }

  createAE() {
    console.log("creating AE");
    var requestBody = {
      "m2m:ae": {
        // Resource Name
        rn: this.options.aeName,
        // Notification URI
        api: "app-sensor",
        // Notificaation Content Type
        lbl: ["Type/sensor", "Category/temperature", "Location/home"],
        rr: "false"
      }
    };
    var protocol;
    if (this.options.https) {
      protocol = "https";
    } else {
      protocol = "http";
    }
    var requestContent = {
      url:
        protocol +
        "://" +
        this.options.host +
        ":" +
        this.options.port +
        "/~/in-cse",
      method: "POST",
      ca: fs.readFileSync(CA_FILE),
      headers: {
        "X-M2M-Origin": this.options.credentials,
        "Content-Type": "application/json;ty=2"
      },
      body: JSON.stringify(requestBody)
    };
    return new Promise(function(resolve, reject) {
      request(requestContent, function(error, response, body) {
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
        rn: "DATA"
      }
    };
    var protocol;
    if (this.options.https) {
      protocol = "https";
    } else {
      protocol = "http";
    }
    var requestContent = {
      url:
        protocol +
        "://" +
        this.options.host +
        ":" +
        this.options.port +
        "/~/in-cse/in-name/" +
        this.options.aeName,
      method: "POST",
      ca: fs.readFileSync(CA_FILE),
      headers: {
        "X-M2M-Origin": this.options.credentials,
        "Content-Type": "application/json;ty=3"
      },
      body: JSON.stringify(requestBody)
    };
    return new Promise(function(resolve, reject) {
      request(requestContent, function(error, response, body) {
        if (error) {
          reject(error);
        } else {
          resolve();
        }
      });
    });
  }
};
