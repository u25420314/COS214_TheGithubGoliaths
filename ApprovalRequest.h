#ifndef APPROVALREQUEST_H
#define APPROVALREQUEST_H

#include <string>

class Task;

class ApprovalRequest{
    private:
        Task* task;
        std::string decision;
        int daysWaiting;

    public:
        ApprovalRequest(Task* task, int daysWaiting);
        Task* getTask() const;

        const std::string& getDecision() const;
        void setDecision(const std::string& decision);

        
        
        int getDaysWaiting() const;
        void setDaysWaiting(int days);
};

#endif