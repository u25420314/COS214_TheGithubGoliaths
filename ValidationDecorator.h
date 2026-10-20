#ifndef VALIDATIONDECORATOR_H
#define VALIDATIONDECORATOR_H


#include <string>
#include <vector>
#include "TaskDecorator.h"

class WorkflowData;

class ValidationDecorator : public TaskDecorator {
    private:
        std::vector<std::string> requiredFields;
        const WorkFlowData* data;
    protected:

        bool canStart() const override;

    public:
        ValidationDecorator(WorkNode* wrapped, const std::vector<std::string& requiredFields);

        void bindData(const WorkflowData* data); //data srouce for the validation instance of workflowdata
        std::vector<std::string> missingFields() const;

        WorkNode* clone{} const override;
};

#endif