const MongoClient = require("mongodb").MongoClient,
  f = require("util").format;

//
//  config
//
var mongoPort = "27017";
var mongoHost = "localhost";
var dbName = "cellulariot";

// Configure the credentials for DEFAULT authentication with the mongo DB
var user = encodeURIComponent("myUserAdmin");
var password = encodeURIComponent("abc123");
var authMechanism = "DEFAULT";

// Connection URL
var mongoUrl = f(
  "mongodb://%s:%s@" +
    mongoHost +
    ":" +
    mongoPort +
    "/" +
    dbName +
    "?authMechanism=%s",
  user,
  password,
  authMechanism
);

//
//  start
//
MongoClient.connect(mongoUrl, { useNewUrlParser: true }, (err, db) => {
  if (err) {
    console.log(err.message);
    return console.log("Error: could not connect to mongodb");
  }
  console.log("database created");

  var dbo = db.db(dbName);
  dbo.createCollection("test", function(err, res) {
    if (err) throw err;
    console.log("Collection created!");
    db.close();
  });
});
