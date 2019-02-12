var assert = require("assert");
var request = require("request");

var host = process.env.wdio__webhost || "127.0.0.1";
var port = process.env.wdio__webport || "3000";
var om2mhost = process.env.wdio__om2mhost || "127.0.0.1";
var om2mport = process.env.wdio__om2mport || "8080";
var isHttps = process.env.wdio__ishttps || "false";

describe("Testing subscription functionality", () => {
  it("Should login into the webapp", () => {
    browser.maximizeWindow();
    browser.url(`${isHttps == "true" ? "https" : "http"}://${host}:${port}`);

    var username = $("input[name=username]"),
      password = $("input[name=password]"),
      submitButton = $("button[type=submit]");

    username.setValue("admin");
    password.setValue("admin");

    submitButton.click();

    var currentPath = browser.execute(() => {
      return location.pathname;
    });

    assert.equal(currentPath, "/");
  });
  it("Should display a resource on the page after sending subscription", () => {
    browser.maximizeWindow();
    var om2mhost_input = $("#om2mhost"),
      om2mport_input = $("#om2mport"),
      webapphost_input = $("#webapphost"),
      webappport_input = $("#webappport"),
      om2mishttps = $("input[name=ishttps]");

    browser.execute(() => {
      var subscribeModalLink = document.querySelector(
        "a.waves-effect.waves-light.btn.modal-trigger"
      );
      if (!!subscribeModalLink) {
        subscribeModalLink.click();
      }
    });

    if (isHttps == "true") {
      om2mishttps.click();
    }

    om2mhost_input.setValue(om2mhost);
    om2mport_input.setValue(om2mport);
    webapphost_input.setValue(host);
    webappport_input.setValue(port);

    browser.execute(() => {
      var subscribeButton = document.querySelector(
        ".modal-close.waves-effect.waves-green.btn-flat"
      );
      if (!!subscribeButton) {
        subscribeButton.click();
      }
    });

    browser.waitUntil(
      () => {
        var isMy_Sensor = false;
        var applicationEntities = $$(".card-title");
        applicationEntities.forEach(ae => {
          if (ae.getText() == "MY_SENSOR") {
            isMy_Sensor = true;
          }
        });
        return isMy_Sensor;
      },
      8000,
      "Expected MY_SENSOR to show up on page after 8s"
    );
  });
  it("Should redirect you to monitoring data page", () => {
    browser.maximizeWindow();
    var monitorDataButton = $(
      "a.waves-effect.waves-light.btn.orange.darken-2.monitor-data"
    );
    monitorDataButton.click();

    var currentPath = browser.execute(() => {
      return location.pathname + location.search;
    });

    assert.equal(currentPath, "/data?ae=MY_SENSOR");
  });
  it("Should see a change in data", () => {
    browser.maximizeWindow();
    var recordButton = $("#resume");
    request.post(
      `${
        isHttps == "true" ? "https" : "http"
      }://${om2mhost}:${om2mport}/~/in-cse/in-name/MY_SENSOR/DATA`,
      {
        headers: {
          "content-type": "application/json;ty=4",
          "x-m2m-origin": "admin:admin"
        },
        body: JSON.stringify({
          "m2m:cin": { cnf: "application/text", con: '{"kWH": "9001" }' }
        })
      },
      function(err, resp, body) {
        if (err) {
          // throw new Error("Unable to post data. ");
        }
        console.log('POST sent', new Date());
      }
    );
    browser.pause(2000);
    browser.execute(() => {
      window.getJSON();
    });
    browser.pause(5000);

    console.log('AE_JSON requsted', new Date());
    var ShouldBeOver9000 = browser.execute(() => {
      return window.AE_JSON[0].data;
    });
    assert.equal("9001", ShouldBeOver9000);
  });
  it("Should delete MY_SENSOR", () => {
    browser.maximizeWindow();
    var goBackButton = $("a.waves-effect.waves-light.btn.red");

    goBackButton.click();

    browser.waitUntil(
      () => {
        return $("a.waves-effect.waves-light.btn.red").isExisting();
      },
      5000,
      "Expected delete button to show up on page after 5s"
    );

    var deleteButton = $("a.waves-effect.waves-light.btn.red");
    deleteButton.click();
    browser.pause(2000);
    assert.equal($(".card-title").isExisting(), false);
  });
});
