var path = require('path');
var fs = require('fs');
const Json2csvParser = require('json2csv').Parser;

module.exports = function(app, isAuthenticatedCustomMiddleware,serverOpt) {
    app.get('/api/download/:ae', isAuthenticatedCustomMiddleware, function(req, res) {
        var resourceName = req.params.ae;
        if(serverOpt.AE_NAMES.includes(resourceName)) {
            if(fs.existsSync(resourceName)) {
                var dataFile = path.resolve(__dirname + '../../../' + resourceName + '/data.json');
                const fields = ['date', 'data'];
                const json2csvParser = new Json2csvParser({fields});

                fs.readFile(dataFile, {encoding: 'utf8'},function(err, data) {
                    const csv = json2csvParser.parse(JSON.parse(data));
                    fs.writeFile(path.resolve(__dirname + '../../../' + resourceName + '/data.csv'),csv, function(err) {
                        if(err) {
                            console.log(err);
                        }else {
                            res.download(path.resolve(__dirname + '../../../' + resourceName + '/data.csv'));
                        }
                    });
                });
                // res.sendfile(dataFile);
            }else {
                res.statusCode = 500;
                res.json({
                    message: 'Error: unable to find'
                });
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