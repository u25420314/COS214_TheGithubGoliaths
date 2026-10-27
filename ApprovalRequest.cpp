#include "ApprovalRequest.h"

ApprovalRequest::ApprovalRequest(Task* task, int daysWaiting): task(task), decision("pending"), daysWaiting(daysWaiting){

}

Task* ApprovalRequest::getTask() const{
    return task;
}

const std::string& ApprovalRequest::getDecision() const{
    return decision;
}

void ApprovalRequest::setDecision(const std::string& decision){
    this->decision = decision;
}

int ApprovalRequest::getDaysWaiting() const{
    return daysWaiting;
}

void ApprovalRequest::setDaysWaiting(int days){
    daysWaiting = days;
}