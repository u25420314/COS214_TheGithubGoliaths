#include "RoleApprover.h"
#include "ApprovalRequest.h"
#include "Task.h"

RoleApprover::RoleApprover(const std::string& role) : role(role){

}
void RoleApprover::handle(ApprovalRequest& request){
    Task* task = request.getTask();
    if(task != NULL && task->getRequiredRole() == role){
        request.setDecision("Approved");
        return;
    }

    if(next != NULL){
        next->handle(request);
    }
}