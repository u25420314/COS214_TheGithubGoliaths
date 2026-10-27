#ifndef TIMEOUTESCALATIONHANDLER_H
#define TIMEOUTESCALATIONHANDLER_H

#include "ApprovalHandler.h"

class TimeoutEscalationHandler : public ApprovalHandler{
    private:
        int limitDays;
    public:
        TimeoutEscalationHandler(int limitDays);
        virtual void handle(ApprovalRequest& request);

};

#endif