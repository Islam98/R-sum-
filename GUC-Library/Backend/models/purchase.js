const mongoose = require("mongoose");
const Schema = mongoose.Schema;
const ITEM = require('./item');

var quantity ;
var name;
const purchaseSchema = new Schema ({
  date:{
    type: Date,
    default: Date.now
  },
  items:[{itemName: { type: String }, quantity: {type:Number, default: 0}}],
  //items:[{name:{type:mongoose.ObjectId, ref:'item' }} , {quantity : {type:Number, default: 0}}],
  paidCheck:{
    type:Boolean,
    default:false
  },
  userName:{
    type:String,
    required:true
  },
  place:{
    type:String,
    enum: ["c1" , "d1" , "b1"]
  },
  totalPrice:{
    type:Number,
    default: 0 
  }
})

module.exports = Purchase = mongoose.model("purchase", purchaseSchema);