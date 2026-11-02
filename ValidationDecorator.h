#ifndef VALIDATIONDECORATOR_H
#define VALIDATIONDECORATOR_H

#include <string>
#include <vector>
#include "TaskDecorator.h"
#include "WorkflowData.h"

/// @brief forward declaration of WorkflowData
class WorkflowData;

/// @brief class for ValidationDecorator
class ValidationDecorator : public TaskDecorator {
    private:
        /// @brief the required fields needed to validate a task
        std::vector<std::string> requiredFields;
        /// @brief the workflowData thats binded to the task
        const WorkflowData* data;

    public:
        /// @brief checks if the decorator can start or not
        /// @return if the decorator can start or not
        bool canStart() const override;
        /// @brief Constructor for the ValidationDecorator
        /// @param wrapped the WorkNode that the decorator wraps
        /// @param requiredFields the fields the decorator needs to do
        ValidationDecorator(WorkNode* wrapped, const std::vector<std::string>& requiredFields);

        /// @brief binds the WorkflowData to the decorator
        /// @param data the data to be binded
        void bindData(const WorkflowData* data); //data srouce for the validation instance of workflowdata
        /// @brief gets the missing fields still required by the decorator
        /// @return the missing fields still required by the decorator
        std::vector<std::string> missingFields() const;

        /// @brief clones the decorator
        /// @return the cloned decorator
        WorkNode* clone() const override;
};

#endif