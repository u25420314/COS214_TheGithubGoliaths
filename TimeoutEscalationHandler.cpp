#include "TimeoutEscalationHandler.h"
#include "ApprovalRequest.h"

TimeoutEscalationHandler::TimeoutEscalationHandler(int limitDays) : limitDays(limitDays){

}

void TimeoutEscalationHandler::handle(ApprovalRequest& request){
    if(request.getDaysWaiting() > limitDays){
        request.setDecision("Escalated");
        return;
    }

    if(next != NULL){
        next->handle(request);
    }
}