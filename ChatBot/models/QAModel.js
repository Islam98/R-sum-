const mongoose = require('mongoose')
const QuestionModel = require('./QuestionModel').schema
const AnswerModel = require('./AnswerModel').schema

const Schema = mongoose.Schema
const QASchema = new Schema({
        Question: QuestionModel,
        Answer: AnswerModel
    
    
})

const QAModel= mongoose.model('QAs', QASchema)
module.exports = QAModel