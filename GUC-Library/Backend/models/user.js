const mongoose = require("mongoose");
require("mongoose-type-email");
const Schema = mongoose.Schema;

const userSchema = new Schema({
  userName: {
    type: String,
    required: true,
    unique: true 
  },
  email: {
    type: String,
    required: true,
    unique: true 
  },
  password:{
    type:String,
    required:true
  },
  isAdmin: {
    type:Boolean,
    required:true,
    default:false
  }

  
});

module.exports = User = mongoose.model("user", userSchema);