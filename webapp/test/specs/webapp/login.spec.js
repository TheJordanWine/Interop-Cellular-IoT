const assert = require("assert");

var host = process.env.wdio__webhost || "127.0.0.1";
var port = process.env.wdio__webport || "3000";
var isHttps = process.env.wdio__ishttps || "false";

describe("Testing the login page", () => {
  it("Should redirect you to login page when not logged in", () => {
    browser.maximizeWindow();
    browser.url(`${isHttps == "true" ? "https" : "http"}://${host}:${port}`);
    var currentPath = browser.execute(() => {
      return location.pathname;
    });
    assert.equal(currentPath, "/login");
  });
  it("Should update url when login fails", () => {
    browser.maximizeWindow();
    var username = $("input[name=username]"),
      password = $("input[name=password]"),
      submitButton = $("button[type=submit]");

    username.setValue("wrongAdmin");
    password.setValue("wrongAdmin");

    submitButton.click();

    var currentPathAndQuery = browser.execute(() => {
      return location.pathname + location.search;
    });

    assert.equal(currentPathAndQuery, "/login?wrong_login");
  });
  it("Should redirect to homepage when login is successful ", () => {
    browser.maximizeWindow();
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
  it("Should redirect to login page when clicking on logout", async () => {
    browser.maximizeWindow();
    var logoutButton = $("a.waves-effect.waves-light.btn.logout");

    logoutButton.click();
    var currentPath = browser.execute(() => {
      return location.pathname;
    });
    assert.equal(currentPath, "/login");
  });
});
