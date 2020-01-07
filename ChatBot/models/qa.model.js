const mongoose = require('mongoose')
const QuestionModel = require('./question.model').schema
const AnswerModel = require('./answer.model').schema

const Schema = mongoose.Schema
const QASchema = new Schema({

    
        Question: QuestionModel,
        Answer: AnswerModel
    
    
})

const QAModel= mongoose.model('QAs', QASchema)
module.exports = QAModel