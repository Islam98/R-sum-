const Joi = require( 'joi');
Joi.objectId = require( 'joi-objectid')(Joi);
module.exports = {
    startSessionValidation: request => {
        const sessionSchema = {
            User: { id: Joi.number().required() },
            Question: { text: Joi.required() }

        }

        return Joi.validate(request, sessionSchema)
    }
}