
const express = require( 'express');
const router = express.Router();
const message = require('../controllers/message.controller')
router.post('/', message.message)
  module.exports = router;
  