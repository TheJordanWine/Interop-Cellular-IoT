// var passport = require('passport');
// var LocalStrategy = require('passport-local').Strategy
// var User = require('../modals/User');

module.exports = function (app,passport,LocalStrategy, User) {
    passport.serializeUser((user, done) => {
        done(null, user.id);
    });

    passport.deserializeUser((id, done) => {
        User.getUserById(id, (err, user) => {
            done(err, user);
        });
    });

    passport.use(new LocalStrategy((username, password, done) => {
        User.getUserByUsername(username, (err, user) => {
            if (err) throw err;
            if (!user) {
                return done(null, false, {
                    message: 'Unknown User'
                });
            }

            User.comparePassword(password, user.password, (err, isMatch) => {
                if (err) return done(err);
                if (isMatch) {
                    return done(null, user);
                } else {
                    done(null, false, {
                        message: 'Invalid Password'
                    });
                }
            });
        });
    }));
    app.get('/login', function (req, res) {
        res.render('login');
    });
    app.get('/register', function (req, res) {
        res.render('register');
    });
    app.post('/register', function (req, res) {
        var password = req.body.password
        var username = req.body.username
      
        var newUser = new User({
          username: username,
          password: password,
        })
      
        User.createUser(newUser, (err, user) => {
          if (err) {
            throw err
          }
          console.log(user)
        })
        res.location('/')
        res.redirect('/')
    });
    app.post('/login', passport.authenticate('local', {
        failureRedirect: '/login?error'
    }), function (req, res) {
        req.session.isAuth = true;
        res.redirect('/');
    });
}