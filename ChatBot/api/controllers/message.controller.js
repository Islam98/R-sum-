
const Session = require( '../../models/session.model');
const Question = require( '../../models/question.model');
const Answer = require( '../../models/answer.model');
const QA = require( '../../models/qa.model');
const service = require( '../../config/watsonAssistant')
const messageValidation = require('../helpers/validations/message.validation')
exports.message = async function(req, res) {
  try{
    if (messageValidation.error) return res.status(400).send({ StatusCode: 1044 });
  const sessionId = req.body.Session.id 
    const dateNow = new Date();
    const sessionFound = await Session.findOne({ _id: sessionId });
    if (!sessionFound) {
      res.status(400).send({ StatusCode: 1011 });
    } else {
      const sessionFound2 = await Session.findOne({
        _id: sessionId,
        endDate: null
      });
      if (!sessionFound2) {
        res.status(404).send({ StatusCode: 1022 });
      } else {
        await service
          .message({
            workspace_id:  'c80c83c6-9cf7-4e8d-b047-6d246281f167 ',
            input: { text: req.body.Question.text }
            //     context:{
            //     'tones':codeInt,
            // 'tonesTypes':tones
            // }
          })
  
          .then(async resp => {
            const question = await Question.create({
              text: req.body.Question.text,
              date: dateNow
            });
            const answer = await Answer.create({
              text: resp.output.text[0],
              date: dateNow
            });
            const qa = await QA.create({
              Question: question,
              Answer: answer
            });
            const SessionUpdating = await Session.findOne({ _id: sessionId });
            const numberOfQuestionAndAnswer =
              SessionUpdating.numberOfQuestionAndAnswer;
            await Session.updateOne(
              { _id: sessionId },
              {
                numberOfQuestionAndAnswer: numberOfQuestionAndAnswer + 1,
                $push: {
                  listOfQA: qa
                }
              }
            );
            const session = await Session.findOne({ _id: sessionId });
            res.send({ StatusCode: 0, Answer:resp.output.text[0]});
          })
          .catch(err => {
            console.log(err);
          });
      }
    }
  } catch (err) {
    res.status(400).send({ StatusCode: 1055 });
  }
  };
  