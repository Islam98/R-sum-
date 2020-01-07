const Joi = require( 'joi');
Joi.objectId = require( 'joi-objectid')(Joi);
module.exports = {
    messageValidation: request => {
        const messageSchema = {
            Session: { id: Joi.objectId().required() },
            Question: { text: Joi.required() }

        }

        return Joi.validate(request, messageSchema)
    }
}