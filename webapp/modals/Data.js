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

module.exports.getData = (obj, cb) => {
    return Data.find(obj);
};

module.exports.getDataByDate = (date, cb) => {
    var query = {
        date: date
    };
    Data.findOne(query, cb);
};