#include "RoleApprover.h"
#include "ApprovalRequest.h"
#include "Task.h"
#include "Participant.h"

RoleApprover::RoleApprover(const std::string& role) : role(role){

}
void RoleApprover::handle(ApprovalRequest& request){
    Task* task = request.getTask();
    Participant* person = (task != NULL) ? task->getAssignee() : NULL;

    if(person != NULL && person->hasRole(role)){
        request.setDecision("Approved");
        return;
    }

    if(next != NULL){
        next->handle(request);
    }
}