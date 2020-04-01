const express = require("express");
const mongoose = require("mongoose");
const router = express.Router();

const User = require('../../models/user');



router.get("/getAllUserNames", (req, res) => {
  User.find().then(users => res.json(users));
});

router.post("/addNewUser" , (req,res) =>{
  const newUser = new User({
    userName:req.body.userName,
    email:req.body.email,
    password:req.body.password
  })
  newUser.save().then(User => res.json(User)) ;
})



  module.exports = router;




  


