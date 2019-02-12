var parseString = require('xml2js').parseString;

module.exports = function (app, saveDataToJSON) {
    /**
     * This is where we'll expect the oneM2M server to post info to.
     */
    app.post('/monitor', function (req, res) {
        //res is the response object
        console.log('test');
        var incomingTemp;
        if (req.body['m2m:sgn'].nev) { // First check if we're actually getting a data notification
            parseString(req.body['m2m:sgn'].nev[0].rep[0].con[0], (err, result) => {
                if (!err) {
                    incomingTemp = result.obj.int[0]['$'].val;
                } else {
                    //get the actual request body content in the form of :  {"temp" : 49}
                    var content = req.body['m2m:sgn'].nev[0].rep[0].con[0];
                    //get the CT data in the form of : 20181123T112401
                    var ct = req.body['m2m:sgn'].nev[0].rep[0].ct[0].match(/(\d{4})(\d{2})(\d{2})(\w)(\d{2})(\d{2})(\d{2})/);
                    //Javscript Object Data with toString functionaility
                    var creationData = new Date(ct[1], ct[2], ct[3], ct[5], ct[6], ct[7]);
                    //Get the application name from the incoming request
                    var AEName = req.body['m2m:sgn'].sur[0].match(/(?<=\/)(.*)(?=\/)/)[1].split('/')[2];
                    saveDataToJSON(AEName, creationData.toUTCString(), content);

                    incomingTemp = JSON.parse(content).kWH;
                }

            });
            console.log("Got voltage of: " + incomingTemp);
        }
        res.status(200).send("thanks!");
    });
    /**
     * Reject anything that is not a post to /monitor
     */
    app.all('/monitor', function(req,res) {
        res.status(405);
        res.render('error', {"message" : "Bad method. POST required"});
    });

}