var request = require('request');

module.exports = function(app,serverOpt) {
    /**
 * Manually delete resource
 */

 app.post('/api/delete', function(req, res) {
    let isHttps = req.body.ishttps,
        om2mhost = req.body.om2mhost,
        om2mport = req.body.om2mport,
        resourceName = req.body.resourceName;
    console.log(isHttps);

    var options = {
        method: 'DELETE',
        url: `${isHttps == 'true' ? 'https' : 'http'}://${om2mhost}:${om2mport}/~/in-cse/in-name/${resourceName}`,
        headers: {
            'X-M2M-Origin': 'admin:admin'
        }
    };
    request(options, function(error, response, body) {
        if (!error && response.statusCode == 200) {
            var tempAE_NAMES = [];
            serverOpt.AE_NAMES.forEach(rn => {
                if(rn != resourceName) {
                    tempAE_NAMES.push(rn);
                }
            });
            serverOpt.AE_NAMES = tempAE_NAMES;
            res.statusCode = 200;
            res.json({
                message: 'Deleted successfully'
            });
        }
        else {
            console.log(body);
            res.statusCode = 500;
            res.json({
                message: 'Unable to complete request'
            });
            console.log(error);
        }
    });
    // DELETE http://127.0.0.1:8080/~/in-cse/in-name/MY_TEST
    // X-M2M-Origin: admin:admin 
 });
}