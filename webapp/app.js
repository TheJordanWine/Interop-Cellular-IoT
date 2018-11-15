var express = require('express');



var app = express();
    app.set('view engine', 'pug');
    app.use(express.static(__dirname + '/public'));
/**
 * signifys a get request for the inital path of the site
 */
app.get('/', function(req, res) {
    //res is the response object
    //render will take string and search for a pug file in views/ and will render it out
    res.render('index');
});

app.get('/test', function(req, res) {
    res.render('test');
})

app.listen(1400, function() {
    console.log('Listening on port 1400...');
    console.log('Head over to localhost:1400');
});
