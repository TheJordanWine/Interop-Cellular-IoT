var express = require('express');



var app = express();
    app.set('view engine', 'pug');

/**
 * signifys a get request for the inital path of the site
 */
app.get('/', function(req, res) {
    //res is the response object
    //render will take string and search for a pug file in views/ and will render it out
    res.render('index');
});

app.listen(3000, function() {
    console.log('Listening on port 3000...');
    console.log('Head over to localhost:3000');
});
