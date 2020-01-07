const express = require( 'express');
const router = express.Router();
const closeSession = require('../controllers/closeSession.controller')
router.post('/', closeSession.closeSession)
router.post('/byUser', closeSession.closeSessionById)
router.post('/allSessions', closeSession.closeAllSessions)

  module.exports = router;
  