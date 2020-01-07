const express = require('express');
const router = express.Router();
const mongoose = require('mongoose');
mongoose.set("useFindAndModify", false);
const Item = require('../../models/item');

// Add new item
router.post('/newItem' , (req,res) =>{
  const newItem = new Item({
    name: req.body.name,
    price: req.body.price,
    description: req.body.description,
    quantity: req.body.quantity
  });

  newItem.save().then(item => res.json(item));
})


//Get all items
router.get('/getItems' , (req,res)=>{
  Item.find()
    .then(items => res.json(items));
});

//Search for a specific item
router.post('/searchByName' , (req,res) =>{
  Item.findOne({name:req.body.name} , (err,obj) =>{
    if(err)
        res.json('NO SUCH ITEM ...')
    else{
      res.json(obj);
    }    
  } )
})

// Update Item Quantity
router.post('/updateQuantity' , (req,res) =>{
  Item.findOneAndUpdate(
    { name: req.body.name },
    { quantity: req.body.quantity },
    { new: true },
    (err, obj) => {
      if (err) res.json("ITEM NOT FOUND ...");
      else res.json(obj);
    }
  );
})


module.exports = router;