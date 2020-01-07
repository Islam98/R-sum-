
const Session = require( '../../models/session.model');
const Question = require( '../../models/question.model');
const Answer = require( '../../models/answer.model');
const QA = require( '../../models/qa.model');
const service = require( '../../config/watsonAssistant')
const startSessionValidation = require('../helpers/validations/startSession.validation')
exports.startSession = async function(req, res) {
  try{   
  if (startSessionValidation.error) return res.status(400).send({ StatusCode: 1044 });
      const StartDate = new Date();
      const sessionFound = await Session.findOne({
        userId: req.body.User.id,
        endDate: null
      });
      console.log(sessionFound);
      if (sessionFound) {
        res.status(400).send({ StatusCode: 1011 });
      } else {
        //   res.send(req.body.text)
        // const tones=[];
        // const text = req.body.text;
        // const toneParams = {
        //   tone_input: { 'text': text },
        //   content_type: 'application/json',
        // };
    
        // await toneAnalyzer.tone(toneParams)
        //   .then(toneAnalysis => {
        //  (toneAnalysis);
        //     tones = toneAnalysis.document_tone.tones
        //   })
        //   .catch(err => {
        //      ('error:', err);
        //   });
        // const codeInt=0;
        //  (tones)
    
        // tones.forEach(myFunction);
    
        // function myFunction(item) {
        //   if(item.tone_name === 'Anger'){
        //     codeInt=1;
        //      (item)
        //   }
        //   else if(item.tone_name === 'Fear'){
        //   codeInt=2;
        // }
        // else if(item.tone_name === 'Confident'){
        //   codeInt=3;
        // }
        // else if(item.tone_name === 'Sadness'){
        //   codeInt=4;
        // }
        // else if(item.tone_name === 'Joy'){
        //   codeInt=5;
        // }
        // else if(item.tone_name === 'Analytical'){
        //   codeInt=6;
        // }
    
        // else if(item.tone_name === 'Tentative'){
        //   codeInt=7;
        // }
    
        //   }
    
        // if(await tones.filter(x => (x.tone_name === 'Anger'))){
        //   codeInt=1;
        //    (x)
        // }
        // else if(await tones.filter(item => (item.tone_name === 'Fear'))){
        //   codeInt=2;
        // }
        // else if(await tones.filter(item => (item.tone_name === 'Confident'))){
        //   codeInt=3;
        // }
        // else if(await tones.filter(item => (item.tone_name === 'Sadness'))){
        //   codeInt=4;
        // }
        // else if(await tones.filter(item => (item.tone_name === 'Joy'))){
        //   codeInt=5;
        // }
        // else if(await tones.filter(item => (item.tone_name === 'Analytical'))){
        //   codeInt=6;
        // }
    
        // else if(await tones.filter(item => (item.tone_name === 'Tentative'))){
        //   codeInt=7;
        // }
        await service
          .message({
            workspace_id: 'c80c83c6-9cf7-4e8d-b047-6d246281f167 ',
            input: { text: req.body.Question.text }
            //     context:{
            //     'tones':codeInt,
            // 'tonesTypes':tones
            // }
          })
    
          .then(async resp => {
            const question = await Question.create({
              text: req.body.Question.text,
              date: StartDate
            });
            const answer = await Answer.create({
              text: resp.output.text[0],
              date: StartDate
            });
            const qa = await QA.create({
              Question: question,
              Answer: answer
            });
    
            session = await Session.create({
              listOfQA: [qa],
              userId: req.body.User.id,
              startDate: StartDate,
              endDate: null,
              duration: null,
              numberOfQuestionAndAnswer: 1
            });
    
            await res.send({ StatusCode: 0, Answer: resp.output.text[0] });
          })
          .catch(err => {
            console.log(err);
          });
          console.log('sasasa')

      }
    } catch (err) {
      res.status(400).send({ StatusCode: 1055 });
    }
   }