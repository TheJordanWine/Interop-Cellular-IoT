var request = require("request");
var fs = require("fs");
var path = require("path");
var rimraf = require("rimraf");
var host = process.env.wdio__webhost || "127.0.0.1";
var port = process.env.wdio__webport || "3000";
var om2mhost = process.env.wdio__om2mhost || "127.0.0.1";
var om2mport = process.env.wdio__om2mport || "8080";
var isHttps = process.env.wdio__ishttps || "false";

var isHeadless = process.env.wdio__headless || 'true',
  browserOpts = {};
if (isHeadless == 'true') {
  browserOpts = {
    maxInstances: 5,
    browserName: "chrome",
    "goog:chromeOptions": {
      args: ["--headless", "--disable-gpu", "window-size=1920,1080"]
    }
  };
} else {
  browserOpts = {
    maxInstances: 5,
    browserName: "chrome",
  };
}


exports.config = {
  //
  // ====================
  // Runner Configuration
  // ====================
  //
  // WebdriverIO allows it to run your tests in arbitrary locations (e.g. locally or
  // on a remote machine).
  runner: "local",
  capabilities: [browserOpts],

  //
  // ==================
  // Specify Test Files
  // ==================
  // Define which test specs should run. The pattern is relative to the directory
  // from which `wdio` was called. Notice that, if you are calling `wdio` from an
  // NPM script (see https://docs.npmjs.com/cli/run-script) then the current working
  // directory is where your package.json resides, so `wdio` will be called from there.
  //
  specs: ["./test/specs/**/*.js"],
  // Patterns to exclude.
  exclude: [
    // 'path/to/excluded/files'
  ],
  //
  // ============
  // Capabilities
  // ============
  // Define your capabilities here. WebdriverIO can run multiple capabilities at the same
  // time. Depending on the number of capabilities, WebdriverIO launches several test
  // sessions. Within your capabilities you can overwrite the spec and exclude options in
  // order to group specific specs to a specific capability.
  //
  // First, you can define how many instances should be started at the same time. Let's
  // say you have 3 different capabilities (Chrome, Firefox, and Safari) and you have
  // set maxInstances to 1; wdio will spawn 3 processes. Therefore, if you have 10 spec
  // files and you set maxInstances to 10, all spec files will get tested at the same time
  // and 30 processes will get spawned. The property handles how many capabilities
  // from the same test should run tests.
  //
  maxInstances: 3,
  //
  // If you have trouble getting all important capabilities together, check out the
  // Sauce Labs platform configurator - a great tool to configure your capabilities:
  // https://docs.saucelabs.com/reference/platforms-configurator
  //
  // capabilities: [
  //   {
  //     browserName: "chrome"
  //   }
  // ],
  //
  // ===================
  // Test Configurations
  // ===================
  // Define all options that are relevant for the WebdriverIO instance here
  //
  // Level of logging verbosity: trace | debug | info | warn | error
  logLevel: "error",
  //
  // Warns when a deprecated command is used
  deprecationWarnings: true,
  //
  // If you only want to run your tests until a specific amount of tests have failed use
  // bail (default is 0 - don't bail, run all tests).
  bail: 0,
  //
  // Set a base URL in order to shorten url command calls. If your `url` parameter starts
  // with `/`, the base url gets prepended, not including the path portion of your baseUrl.
  // If your `url` parameter starts without a scheme or `/` (like `some/path`), the base url
  // gets prepended directly.
  baseUrl: "http://localhost",
  //
  // Default timeout for all waitFor* commands.
  waitforTimeout: 10000,
  //
  // Default timeout in milliseconds for request
  // if Selenium Grid doesn't send response
  connectionRetryTimeout: 90000,
  //
  // Default request retries count
  connectionRetryCount: 3,
  //
  // Test runner services
  // Services take over a specific job you don't want to take care of. They enhance
  // your test setup with almost no effort. Unlike plugins, they don't add new
  // commands. Instead, they hook themselves up into the test process.
  services: ["selenium-standalone"],
  //
  // Framework you want to run your specs with.
  // The following are supported: Mocha, Jasmine, and Cucumber
  // see also: https://webdriver.io/docs/frameworks.html
  //
  // Make sure you have the wdio adapter package for the specific framework installed
  // before running any tests.
  framework: "mocha",
  //
  // Test reporter for stdout.
  // The only one supported by default is 'dot'
  // see also: https://webdriver.io/docs/dot-reporter.html
  reporters: ["spec"],

  //
  // Options to be passed to Mocha.
  // See the full list at http://mochajs.org/
  mochaOpts: {
    ui: "bdd",
    timeout: 600000
  },
  //
  // =====
  // Hooks
  // =====
  // WebdriverIO provides several hooks you can use to interfere with the test process in order to enhance
  // it and to build services around it. You can either apply a single function or an array of
  // methods to it. If one of them returns with a promise, WebdriverIO will wait until that promise got
  // resolved to continue.
  /**
   * Gets executed once before all workers get launched.
   * @param {Object} config wdio configuration object
   * @param {Array.<Object>} capabilities list of capabilities details
   */
  // onPrepare: function (config, capabilities) {
  // },
  /**
   * Gets executed just before initialising the webdriver session and test framework. It allows you
   * to manipulate configurations depending on the capability or spec.
   * @param {Object} config wdio configuration object
   * @param {Array.<Object>} capabilities list of capabilities details
   * @param {Array.<String>} specs List of spec file paths that are to be run
   */
  beforeSession: function (config, capabilities, specs) {
    request.post('https://hooks.slack.com/services/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', {
      headers: {
        "content-type": "application/json"
      },
      body: JSON.stringify({
        text: `Travis build starting... [${process.env.TRAVIS_BUILD_ID}]: <${process.env.TRAVIS_BUILD_WEB_URL}|view here>`,
        username: "TravisBot",
        color: "#D00000",
        fields: [{
          "title": `Commit: ${process.env.TRAVIS_COMMIT}`,
          "value": `${process.env.TRAVIS_COMMIT_MESSAGE}\n`,
          "short": false
        }]
      })
    });
    var mySensorDir = path.resolve(__dirname, "../MY_SENSOR");
    if (fs.existsSync(mySensorDir)) {
      rimraf.sync(mySensorDir);
    }
    request.post(
      `${
        isHttps == "true" ? "https" : "http"
      }://${om2mhost}:${om2mport}/~/in-cse`, {
        headers: {
          "content-type": "application/xml;ty=2",
          "x-m2m-origin": "admin:admin"
        },
        body: '<m2m:ae xmlns:m2m="http://www.onem2m.org/xml/protocols" rn="MY_SENSOR" ><api>app-sensor</api><lbl>Type/sensor Category/temperature Location/home</lbl><rr>false</rr></m2m:ae>'
      },
      function (err, resp, body) {
        if (err) {
          throw new Error(
            "Unable to create a resource. Please check a OM2M server is configured correctly"
          );
        }
        request.post(
          `${
            isHttps == "true" ? "https" : "http"
          }://${om2mhost}:${om2mport}/~/in-cse/in-name/MY_SENSOR`, {
            headers: {
              "content-type": "application/xml;ty=3",
              "x-m2m-origin": "admin:admin"
            },
            body: '<m2m:cnt xmlns:m2m="http://www.onem2m.org/xml/protocols" rn="DATA"></m2m:cnt>'
          },
          function (err, resp, body) {
            if (err) {
              throw new Error("Unable to create data container. ");
            }
          }
        );
      }
    );
  },
  /**
   * Gets executed before test execution begins. At this point you can access to all global
   * variables like `browser`. It is the perfect place to define custom commands.
   * @param {Array.<Object>} capabilities list of capabilities details
   * @param {Array.<String>} specs List of spec file paths that are to be run
   */
  // before: function (capabilities, specs) {
  // },
  /**
   * Runs before a WebdriverIO command gets executed.
   * @param {String} commandName hook command name
   * @param {Array} args arguments that command would receive
   */
  // beforeCommand: function (commandName, args) {
  // },

  /**
   * Hook that gets executed before the suite starts
   * @param {Object} suite suite details
   */
  // beforeSuite: function (suite) {
  // },
  /**
   * Function to be executed before a test (in Mocha/Jasmine) or a step (in Cucumber) starts.
   * @param {Object} test test details
   */
  // beforeTest: function (test) {
  // },
  /**
   * Hook that gets executed _before_ a hook within the suite starts (e.g. runs before calling
   * beforeEach in Mocha)
   */
  // beforeHook: function () {
  // },
  /**
   * Hook that gets executed _after_ a hook within the suite starts (e.g. runs after calling
   * afterEach in Mocha)
   */
  // afterHook: function () {
  // },
  /**
   * Function to be executed after a test (in Mocha/Jasmine) or a step (in Cucumber) starts.
   * @param {Object} test test details
   */
  // afterTest: function (test) {
  // },
  /**
   * Hook that gets executed after the suite has ended
   * @param {Object} suite suite details
   */
  // afterSuite: function (suite) {
  // },

  /**
   * Runs after a WebdriverIO command gets executed
   * @param {String} commandName hook command name
   * @param {Array} args arguments that command would receive
   * @param {Number} result 0 - command success, 1 - command error
   * @param {Object} error error object if any
   */
  // afterCommand: function (commandName, args, result, error) {
  // },
  /**
   * Gets executed after all tests are done. You still have access to all global variables from
   * the test.
   * @param {Number} result 0 - test pass, 1 - test fail
   * @param {Array.<Object>} capabilities list of capabilities details
   * @param {Array.<String>} specs List of spec file paths that ran
   */
  // after: function (result, capabilities, specs) {
  // },
  /**
   * Gets executed right after terminating the webdriver session.
   * @param {Object} config wdio configuration object
   * @param {Array.<Object>} capabilities list of capabilities details
   * @param {Array.<String>} specs List of spec file paths that ran
   */
  // afterSession: function (config, capabilities, specs) {
  // },
  /**
   * Gets executed after all workers got shut down and the process is about to exit.
   * @param {Object} exitCode 0 - success, 1 - fail
   * @param {Object} config wdio configuration object
   * @param {Array.<Object>} capabilities list of capabilities details
   * @param {<Object>} results object containing test results
   */
  onComplete: function (exitCode, config, capabilities, results) {

    if (results.finished == results.passed) {
      console.log('passed!');
      request.post('https://hooks.slack.com/services/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', {
        headers: {
          "content-type": "application/json"
        },
        body: JSON.stringify({
          text: `Travis build finished. [${process.env.TRAVIS_BUILD_ID}]: <${process.env.TRAVIS_BUILD_WEB_URL}|view here>`,
          username: "TravisBot",
          color: "#D00000",
          fields: [{
            "title": `Results`,
            "value": `All tests are passed!`,
            "short": false
          }]
        })
      });
    } else if (results.failed > 0) {
      request.post('https://hooks.slack.com/services/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', {
        headers: {
          "content-type": "application/json"
        },
        body: JSON.stringify({
          text: `Travis build finished. [${process.env.TRAVIS_BUILD_ID}]: <${process.env.TRAVIS_BUILD_WEB_URL}|view here>`,
          username: "TravisBot",
          color: "#D00000",
          fields: [{
            "title": `Results`,
            "value": `${results.failed} test(s) failed`,
            "short": false
          }]
        })
      });
    }
  },
  /**
   * Gets executed when a refresh happens.
   * @param {String} oldSessionId session ID of the old session
   * @param {String} newSessionId session ID of the new session
   */
  //onReload: function(oldSessionId, newSessionId) {
  //}
};