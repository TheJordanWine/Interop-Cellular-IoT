// var path = require('path');
// var fs = require('fs');
var MongoClient = require('mongodb').MongoClient;

module.exports = function(app, isAuthenticatedCustomMiddleware, Data) {
    app.get('/api/getJSON', isAuthenticatedCustomMiddleware, function(req, res) {
        
        var url = "mongodb://localhost:27017/";
        
        MongoClient.connect(url, function(err, db) {
          if (err) throw err;
          var dbo = db.db("nodeauth");
          var query = { date: new Date().toLocaleDateString() };
          dbo.collection("datas").find(query).toArray(function(err, result) {
            if (err) {
                res.json({
                    message: 'error'
                });
            }else {
                console.log(result);
                res.json(result);
                db.close();
            }
          });
        });

    });
};