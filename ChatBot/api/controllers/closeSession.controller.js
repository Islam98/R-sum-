const Session = require( '../../models/session.model');
const closeSessionValidation = require('../helpers/validations/closeSession.validation')

exports.closeSession = async function(req, res) {
    try {
    
      if (closeSessionValidation.closeSession.error) return res.status(400).send({ StatusCode: 1044 });
  
      const EndDate = new Date();
  
      const sessionId = req.body.Session.id;
      const sessionFound = await Session.findOne({ _id: sessionId });
      if (!sessionFound) {
        res.status(400).send({ StatusCode: 1066 });
      } else {
        const sessionFound2 = await Session.findOne({
          _id: sessionId,
          endDate: null
        });
        if (!sessionFound2) {
          res.status(404).send({ StatusCode: 1077 });
        } else {
          const session = await Session.findOne({ _id: sessionId });
  
          //  const mindiff = EndDate.getMinutes() - session.startDate.getMinutes();
          //  const hourdiff = session.startDate.getHours() ;
          //  if(mindiff < 0)
          //  {
          //   hourdiff = hourdiff - 1 ;
          //   mindiff += 60 ;
          //  }
          //  else{
          //  hourdiff = EndDate.getHours() - session.startDate.getHours();
          //  }
          const diff = Date.now() - session.startDate.getTime();
          const hours =Math.floor(diff / 3600000);
          const minutes =  Math.floor((diff / 3600000-hours)*60)
        const seconds = (((diff / 3600000-hours)*60-minutes)*60)
          
          const duration = {
            hours: hours,
            minutes: minutes,
            seconds: seconds
            //  if(endDate.getDate() < startDate.getDay())
            //  {
            //   new Date(new Date().getTime() - 2*24*(60*diff)*1000).toLocaleDateString()
            //  }.

          };
          console.log(duration);
          await Session.updateOne(
            { _id: sessionId },
            { endDate: EndDate, duration: duration }
          );
          res.send({ StatusCode: 0 });
        }
      }
    } catch (err) {
      res.status(400).send(err.message);
    }
  }

exports.closeSessionById = async function(req, res) {

    try {
     
      if (closeSessionValidation.closeSessionById.error) return res.status(400).send({ StatusCode: 1044 });
  
      const EndDate = new Date();
  
      const userId = req.body.User.id;
      const sessionFound = await Session.findOne({ 'userId': userId , });
  
      if (!sessionFound) {
        res.status(400).send({ StatusCode: 1066 });
      } else {
        const sessionFound2 = await Session.findOne({
          'userId': userId,
          endDate: null
        });
        if (!sessionFound2) {
          res.status(404).send({ StatusCode: 1077 });
        } else {
          const session = await Session.findOne({ 'userId': userId,
          endDate: null });
          const diff = Date.now() - session.startDate.getTime();
          const hours =Math.floor(diff / 3600000);
        const minutes =  Math.floor((diff / 3600000-hours)*60)
         const seconds = (((diff / 3600000-hours)*60-minutes)*60)
          const duration = {
            hours: hours,
            minutes: minutes,
            seconds: seconds
  
          };
          console.log(duration);
          await Session.updateOne(
            { _id: session._id },
            { endDate: EndDate, duration: duration }
          );
          res.send({ StatusCode: 0 });
        }
      }
    } catch (err) {
      res.status(400).send(err.message);
    }
  }
exports.closeAllSessions = async function(req, res) {

    try {
     
      if (closeSessionValidation.closeSessionById.error) return res.status(400).send({ StatusCode: 1044 });
  
      const EndDate = new Date();

      const sessionFound = await Session.find({ 'endDate': null  });
  
      if (!sessionFound) {
        res.status(400).send({ StatusCode: 1066 });
      } else {
       
      sessionFound.forEach(myFunction);

    async function myFunction(item) {
    const diff = Date.now() - item.startDate.getTime();
    const hours =Math.floor(diff / 3600000);
  const minutes =  Math.floor((diff / 3600000-hours)*60)
   const seconds = (((diff / 3600000-hours)*60-minutes)*60)
    const duration = {
      hours: hours,
      minutes: minutes,
      seconds: seconds

    };
    console.log(duration);
    await Session.updateOne(
      { _id: item._id },
      { endDate: EndDate, duration: duration }
    );
    res.send({ StatusCode: 0 });
  }

}
      
    } catch (err) {
      res.status(400).send(err.message);
    }
  }