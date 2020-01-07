const mongoose = require('mongoose')
const Schema = mongoose.Schema
const AnswerSchema = new Schema({
   
    text: {
        type: String,
        required: true
    },
    date: {
        type: Date
    }
})

const Answer= mongoose.model('answers', AnswerSchema)
module.exports = Answer