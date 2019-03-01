var path = require('path');
var fs = require('fs');
var MongoClient = require('mongodb').MongoClient;
module.exports = function (app, isAuthenticatedCustomMiddleware, serverOpt) {
    app.get('/api/get/:ae', isAuthenticatedCustomMiddleware, function (req, res) {
        var resourceName = req.params.ae;

        if (process.env.BETA) {

            var url = "mongodb://localhost:27017/";

            MongoClient.connect(url, function (err, db) {
                if (err) throw err;
                var dbo = db.db("nodeauth");
                var query = {
                    date: !!req.query.currentDate ? req.query.currentDate : new Date().toLocaleDateString()
                };
                dbo.collection("datas").find(query).toArray(function (err, result) {
                    if (err) {
                        res.json({
                            message: 'error'
                        });
                    } else {
                        console.log(result);
                        res.json(result);
                        db.close();
                    }
                });
            });
            
        } else {
            if (serverOpt.AE_NAMES.includes(resourceName)) {
                if (fs.existsSync(resourceName)) {
                    var dataFile = path.resolve(__dirname + '../../../' + resourceName + '/data.json');
                    console.log('filename:', dataFile);
                    res.sendfile(dataFile);
                } else {
                    res.statusCode = 500;
                    res.send('null');
                    res.end();
                }
            } else {
                res.statusCode = 400;
                res.send('null');
                res.end();
            }
        }

    });
}