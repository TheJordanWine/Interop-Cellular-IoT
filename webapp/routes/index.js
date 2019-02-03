module.exports = function (app, isAuthenticatedCustomMiddleware, serverOpt) {
    /*
     * signifies a get request for the inital path of the site
     */
    app.get('/', isAuthenticatedCustomMiddleware, function (req, res) {
        //res is the response object
        //render will take string and search for a pug file in views/ and will render it out
        res.status(200);
        res.render('index', {
            AENames: serverOpt.AE_NAMES
        });
    });
};