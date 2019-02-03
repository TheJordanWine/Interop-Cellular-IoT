module.exports = function(app) {
    app.get('/logout', function(req, res) {
        if(!!req.session && !!req.session.isAuth) {
            req.session.isAuth = undefined;
        }
        res.redirect('/login');
    });
}