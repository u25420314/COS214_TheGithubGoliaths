#ifndef APPROVALREQUEST_H
#define APPROVALREQUEST_H

#include <string>

class Task;


/**
 * @class ApprovalRequest
 * @brief Represents a request for approval of a task
 * 
 * This class stores information about a task to be processed by the chain of resposibility
 * 
 * each approval contains a pointer to the task, 
 * its current approval decision and the number of days its been waiting
 * 
 * ApprovalHandlers use this information to determine 
 * whetehr a rewquest shoulf be approved, escalted, or passed to the next handler in the chain
 */
class ApprovalRequest{
    private:
        Task* task;
        std::string decision;
        int daysWaiting;

    public:
        /**
         * @brief Constructs a approval request for a task
         * 
         * initialises the request witht the specified task and number of days waiting
         * the initial decison is set to "pending"
         * 
         * @param task Pointer to the task requiring approval
         * @param daysWaiting Number of days the request has been waiting
         */
        ApprovalRequest(Task* task, int daysWaiting);
        /**
         * @brief Retrieves the task of the approvalRequest
         * 
         * @return Task* Pointer to the task requiring approval
         */
        Task* getTask() const;
        /**
         * @brief Retrieves the current Decision 
         * 
         * @return const std::string& reference to the current decision string
         */
        const std::string& getDecision() const;
        /**
         * @brief Updates the approval decision of the request
         * 
         * Allows an approval handler to change the decision
         * whent he request is processed
         * 
         * @param decision the new approval decision
         */
        void setDecision(const std::string& decision);

        
        /**
         * @brief Retrieves the number of days the request has been waiting
         * 
         * @return int Number of days the approval request has been waiting
         */
        int getDaysWaiting() const;

        /**
         * @brief Updates the days waiting of the request
         * 
         * This value can be used by escalation handlers to determine whether the request has exceeded  a waiting -time threshold
         * 
         * @param daysWaiting the updated number of days waiting
         */
        void setDaysWaiting(int days);
};

#endif