const Joi = require( 'joi');
Joi.objectId = require( 'joi-objectid')(Joi);
module.exports = {
    closeSession: request => {
        const closeSessionSchema = {
            Session: { id: Joi.objectId().required() }
        }

        return Joi.validate(request, closeSessionSchema)
    },
    closeSessionById: request => {
        const closeSessionByIdSchema = {
            User: { id: Joi.number().required() }
        }

        return Joi.validate(request, closeSessionByIdSchema)
    },
    
}