module.exports = function(app, isAuthenticatedCustomMiddleware) {
    app.get('/data', isAuthenticatedCustomMiddleware, function(req, res) {
        res.render('data', {
            AEName: req.query.ae
        });
    });
}