
const express = require( 'express');
const router = express.Router();
const startSession = require('../controllers/startSession.controller')
router.post('/', startSession.startSession)
  module.exports = router;
  