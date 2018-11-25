module.exports = function(app) {
    app.get('/login', function(req, res) {
        res.render('login');
    });
    app.post('/login', function(req, res) {
        if(req.body.username == 'admin' && req.body.password == 'admin') {
            req.session.isAuth = true;
            res.redirect('/');
        }else {
            res.redirect('/login?wrong_login')
        }
    });
}