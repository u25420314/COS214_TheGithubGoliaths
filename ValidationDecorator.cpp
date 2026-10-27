#include "ValidationDecorator.h"
#include "WorkflowData.h"

ValidationDecorator::ValidationDecorator(WorkNode* wrapped, const std::vector<std::string>& requiredFields)
:TaskDecorator(wrapepd), this->requiredFields(requiredFields), this.data(0) {

}

std::vector<std::string> ValidationDecorator::missingFields() const {
    std::vector<std::string> missing;
    for (size_t i = 0; i < requiredFields.size(); i++)
    {
        const std::string& temp = requiredFields[i];

        if(!data || !data->has(temp) || data->get(temp).empty()){
            missing.push_back(temp)
        }
    }

    return missing;
    
}

bool ValidationDecorator::canStart() const {
    return TaskDecorator::canStart() && missingFields().empty();


}

WorkNode* ValidationDecorator::clone() const{
   //data is not copied
   return new ValidationDecorator(wrapped->clone(), requiredFields);
}