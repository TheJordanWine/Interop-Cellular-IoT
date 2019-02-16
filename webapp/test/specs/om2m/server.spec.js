var assert = require('assert');

var om2mhost = process.env.wdio__om2mhost || "127.0.0.1";
var om2mport = process.env.wdio__om2mport || "8080";
var isHttps = process.env.wdio__ishttps || "false";

describe('Testing the OM2M Webpage', () => {
    it('Should be redirected to login page', (done) => {
        browser.url(`${isHttps == "true" ? "https" : "http"}://${om2mhost}:${om2mport}/webpage`);
        var headerExists = browser.execute(() => {
            var h2Header = document.querySelector('h2');
            if(!!h2Header) {
                return h2Header.innerHTML == "OM2M CSE Resource Tree";
            }else {
                return false;
            }
        });
        assert.equal(true, headerExists);     
    });  
});