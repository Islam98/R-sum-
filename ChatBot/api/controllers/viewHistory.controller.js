
const Session = require( '../../models/session.model');
const viewHistoryValidation = require('../helpers/validations/viewHistory.validation')

exports.viewSession = async function(req, res) {
    try {
     
      if (viewHistoryValidation.viewHistoryValidation.error) return res.status(400).send({ StatusCode: 1044 });
  
      const sessionId = req.body.Session.id;
      const sessionFound = await Session.findOne({ _id: sessionId });
      if (!sessionFound) {
        res.status(400).send({ StatusCode: 1066 });
      } else {
        res.send({ StatusCode: 0, Session:{ 'duration':sessionFound.duration} });
      }
    } catch (err) {
      res.status(400).send({ StatusCode: 1055 });
    }
  };
  
  exports.viewAllSession = async function(req, res) {
    try {
        if (!req.body) { const sessions = await Session.find({},'duration numberOfQuestionAndAnswer userId')
        res.json(sessions)
    return;
    }
  
        if (viewHistoryValidation.viewHistoryAllValidation.error) return res.status(400).send({ StatusCode: 1044 });
  
        const sessions = await Session.find({},'duration numberOfQuestionAndAnswer userId').limit(req.body.numberOfInstances)
    res.json(sessions);
} catch (err) {
    res.status(400).send({ StatusCode: 1055 });
  }
  };