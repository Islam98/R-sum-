const express = require("express");
const router = express.Router();

const Purchase = require('../../models/purchase');

// Add New Purchase
router.post("/newPurchase", (req, res) => {
  const newPurchase = new Purchase({
    items: req.body.items,
    userName: req.body.userName,
    place: req.body.place,
    totalPrice: req.body.totalPrice
    
  });
  newPurchase.save().then(purchase => res.json(purchase));
});

//Get all Purchases
router.post("/getAllPurchase" , (req,res) =>{
  Purchase.find()
    .then(purchases => res.json(purchases));
})

//Get purchases for a certain user
router.post("/getPurchaseForUser" , (req,res)=>{
  Purchase.find({userName:req.body.userName} , (err , objs)=>{
    if(err)
      res.json("SOMETHING WENT WRONG...");
    else
      res.json(objs);  
  });
})

//Update if user paid his purchase 
router.post("/paid" , (req,res) =>{
  Purchase.findOneAndUpdate({ date: req.body.date }, { paidCheck : true} , {new : true} , (err,obj)=>{
    if (err) 
      res.json("SOMETHING WENT WRONG...");
    else 
      res.json(obj);
      
  });
})


module.exports = router;