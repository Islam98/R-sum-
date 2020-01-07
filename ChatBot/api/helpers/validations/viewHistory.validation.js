

const Joi = require('joi');
Joi.objectId = require('joi-objectid')(Joi);
module.exports = {
    viewHistoryValidation: request => {
        const viewHistorySchema = {
            Session: { id: Joi.objectId().required() }

        }

        return Joi.validate(request, viewHistorySchema)
    }
    ,viewHistoryAllValidation: request => {
        const viewHistoryAllSchema = {
            numberOfInstances: Joi.number()

        }
        return Joi.validate(request, viewHistoryAllSchema)
    
}
}
