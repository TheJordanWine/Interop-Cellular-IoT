var request = require('request');

module.exports = function name(app, isAuthenticatedCustomMiddleware, serverOpt) {
    app.get('/status', isAuthenticatedCustomMiddleware, function(req, res) {
        var options = {
            url: `${serverOpt.IS_HTTPS ? 'https' : 'http'}://${serverOpt.ONE_M2M_HOST}:${serverOpts.ONE_M2M_PORT}/~/in-cse`,
            headers: {
              'X-M2M-Origin': 'admin:admin',
              'Accept': 'application/json'
            }
          };
        request(options, function(error, response, body) {
            if (!error && response.statusCode == 200) {
                res.end('true');
            }
            else {
                res.end('false');
            }
        });
    });
}