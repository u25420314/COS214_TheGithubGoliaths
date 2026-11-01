#ifndef APPROVALREQUEST_H
#define APPROVALREQUEST_H

#include <string>

/// @brief Forward declaration of the Task class
class Task;

/// @brief Class representing an approval request for a task
class ApprovalRequest{
    private:
        /// @brief Pointer to the task associated with the approval request
        Task* task;
        /// @brief Decision made on the approval request
        std::string decision;
        /// @brief Number of days the request has been waiting for approval
        int daysWaiting;

    public:
        /// @brief Constructor for ApprovalRequest
        /// @param task 
        /// @param daysWaiting 
        ApprovalRequest(Task* task, int daysWaiting);

        /// @brief gets the task associated with the approval request
        /// @return the task associated with the approval request
        Task* getTask() const;

        /// @brief gets the decision made on the approval request
        /// @return the decision made on the approval request
        const std::string& getDecision() const;
        void setDecision(const std::string& decision);

        /// @brief gets the number of days the request has been waiting for approval
        /// @return the number of days the request has been waiting for approval
        int getDaysWaiting() const;

        /// @brief sets the number of days the request has been waiting for approval
        /// @param days the number of days the request has been waiting for approval
        void setDaysWaiting(int days);
};

#endif