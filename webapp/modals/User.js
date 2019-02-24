const mongoose = require('mongoose');
const bcrypt = require('bcryptjs');

mongoose.connect('mongodb://localhost/nodeauth');


//User Schema
const UserSchema = mongoose.Schema({
    username: {
        type: String,
        index: true
    },
    password: {
        type: String
    }
})

const User = module.exports = mongoose.model('User', UserSchema)

module.exports.getUserById = (id, cb) => {
    User.findById(id, cb);
};

module.exports.getUserByUsername = (username, cb) => {
    var query = {
        username: username
    };
    User.findOne(query, cb);
};
module.exports.comparePassword = (candidatePassword, hash, cb) => {
    bcrypt.compare(candidatePassword, hash, (err, isMatch) => {
        cb(null, isMatch);
    });
};
module.exports.createUser = (newUser, cb) => {
    bcrypt.genSalt(10, (err, salt) => {
        bcrypt.hash(newUser.password, salt, (err, hash) => {
            newUser.password = hash;
            newUser.save(cb);
        });
    });
};