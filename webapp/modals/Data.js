const mongoose = require('mongoose');

mongoose.connect('mongodb://localhost/nodeauth');


//Data Schema
const DataSchema = mongoose.Schema({
    date: {
        type: String,
        index: true
    },
    data: {
        type: String
    }
})

const Data = module.exports = mongoose.model('Data', DataSchema)

module.exports.getData = (id, cb) => {
    return Date.find();
};

module.exports.getDataByDate = (date, cb) => {
    var query = {
        date: date
    };
    Data.findOne(query, cb);
};