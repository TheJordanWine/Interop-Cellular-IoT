var path = require('path');
var fs = require('fs');
module.exports = function(app, isAuthenticatedCustomMiddleware,serverOpt) {
    app.post('/api/get/:ae', isAuthenticatedCustomMiddleware, function(req, res) {
        var resourceName = req.params.ae;
        console.log('test??');
        if(serverOpt.AE_NAMES.includes(resourceName)) {
            if(fs.existsSync(resourceName)) {
                var dataFile = path.resolve(__dirname + '../../../' + resourceName + '/data.json');
                console.log('filename:', dataFile);
                res.sendfile(dataFile);
            }else {
                res.statusCode = 500;
                res.send('null');
                res.end();
            }
        }else {
            res.statusCode = 400;
            res.send('null');
            res.end();
        }
    });
}