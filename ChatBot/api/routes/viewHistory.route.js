
const express = require( 'express');
const router = express.Router();
const viewHistory = require('../controllers/viewHistory.controller')
router.post('/', viewHistory.viewSession)
router.post('/all', viewHistory.viewAllSession) 
module.exports = router;
  