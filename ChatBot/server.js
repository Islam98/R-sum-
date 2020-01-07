
const express = require('express');
const all_routes = require('express-list-endpoints')
const mongoose = require('mongoose')
const db = require('./config/keys').mongoURI
mongoose
    .connect(db, { useNewUrlParser: true })
    .then(() => console.log('Connected to MongoDB'))
    .catch(err => console.log(err))
    mongoose.set('useFindAndModify', false);
var cors=require('cors');
const app = express();
const explore = (req, res) => {
    const routes = all_routes(app)
    const result = {
      ServiceList: []
    }
    for (route of routes) {
      const name = route.path.split('/')[4]
      result.ServiceList.push({
        Service: {
          name: name,
          fullUrl: `http://localhost:3333/${route.path}`
        }
      })
    }
    return res.json(result)
   }
app.use(cors());
app.use(express.json())
app.use(express.urlencoded({extended:false}))
app.use('/explore', explore)
const startSession=require('./api/routes/startSession.route');
app.use('/api/v1/startSession', startSession);

const message=require('./api/routes/message.route');
app.use('/api/v1/message', message);

const viewHistory=require('./api/routes/viewHistory.route');
app.use('/api/v1/viewHistory', viewHistory);
   
const closeSession=require('./api/routes/closeSession.route');
app.use('/api/v1/closeSession', closeSession);
   
const port = process.env.PORT || 3333

app.listen(port, () => console.log(`Server on ${port}`))
