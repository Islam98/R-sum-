const mongoose = require('mongoose');
mongoose.set("useCreateIndex", true);
const Schema = mongoose.Schema;

const ItemSchema = new Schema({
  name: {
    type:String,
    required:true,
    unique: true
  },
  price:{
    type: Number,
    required:true
  },
  description:{
    type:String,
    required:true
  },
  date:{
    type: Date,
    default: Date.now
  },
  quantity:{
    type:Number,
    required:true
  }
})

module.exports = Item = mongoose.model('item' , ItemSchema);