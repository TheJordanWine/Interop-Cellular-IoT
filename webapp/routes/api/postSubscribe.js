var request = require('request');

module.exports = function(app,subscribeToServer,serverOpts) {
    
/**
 * Manual subscription from webapp
 */
app.post('/api/subscribe', function(req, res) {
    /**
     * Posted data
     */
    let isHttps = req.body.ishttps,
        om2mhost = req.body.om2mhost,
        om2mport = req.body.om2mport,
        listenaddr = req.body.listenaddr,
        listenport = req.body.listenport;

    var options = {
        url: `${isHttps ? 'https' : 'http'}://${om2mhost}:${om2mport}/~/in-cse/?rcn=5`,
        headers: {
            'X-M2M-Origin': 'admin:admin',
            'Accept': 'application/json'
        }
    };
    request(options, function(error, response, body) {
        if (!error && response.statusCode == 200) {
            try {
                var CSEBase = JSON.parse(body)["m2m:cb"],
                    childrenResources = CSEBase.ch,
                    applicationEntityResources = childrenResources.filter(ae => ae.ty === 2).map(ae => ae.rn);
                
                serverOpts.AE_NAMES = applicationEntityResources;
                AE_SUBSCRIPTIONS = [];
                serverOpts.AE_NAMES.forEach((aeName) => {
                    AE_SUBSCRIPTIONS.push(subscribeToServer(aeName, {
                        port: om2mport,
                        host: om2mhost,
                        listenAddress : listenaddr,
                        listenPort : listenport,
                        aeName : aeName,
                        https : !!isHttps ? true : false,
                        listenRoute : "/monitor"
                    }));
                });
                Promise.all(AE_SUBSCRIPTIONS).then(function(values) {
                    res.json({
                        message: 'success'
                    });
                });
            }catch(e) {
                console.log(e);
                res.statusCode = 500;
                res.json({
                    message: 'Error unable to grab resources from subscription settings, check logs'
                });
                
            }   
        }else {
            console.log('error');
            res.statusCode = 400;
            res.json({
                message: 'Bad request'
            });
        }
    });
});

}