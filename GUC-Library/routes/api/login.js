const express = require("express");
const nodemailer = require('nodemailer');
const router = express.Router();
const user = require("../../models/user");

// MAIL OPTIONS
 var transporter = nodemailer.createTransport({
   service: "gmail",
   auth: {
     user: "mogamrElshaba7@gmail.com",
     pass: 'Mm01064882098'
   }
 });

 var mailOptions = {
   from: "mogamrElshaba7@gmail.com",
   to: "islamsweify@gmail.com",
   subject: "Nodemailer test",
   text: "Hello MOSTAFA salmly 3la!!"
 };


 // LOGIN

router.post('/login' , (req,res)=>{
  var username = req.body.userName;
  var userpass = req.body.password
  user.findOne({userName:username, password:userpass} , (err,obj) =>{
    if(obj){
      res.json(obj);
    } 
    else{
       res.json("You need to register by email...");
    }
    
  } )

  //REGISTER
})

router.post("/login2", (req, res) => {
  transporter.sendMail(mailOptions, function(err, response) {
    if (err) {
      res.json("Error");
    } else {
      res.json("Email Sent");
    }
  });
});






  module.exports = router;