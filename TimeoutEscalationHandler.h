#ifndef TIMEOUTESCALATIONHANDLER_H
#define TIMEOUTESCALATIONHANDLER_H

#include "ApprovalHandler.h"

/**
 * @class TimeoutEscalationHandler
 * @brief Escalates approval requests that exceed a waiting time limit
 * 
 * this class is concretehandler in the chain or resposnibility
 * 
 * it exmaines the number of days an approved request has been waiting and
 * compares it against a configured time limit
 * 
 * if the request time is exceeded the limit the requetss decision will be changed to escalated
 */
class TimeoutEscalationHandler : public ApprovalHandler{
    private:
        int limitDays;
    public:
        /**
         * 
         * @brief Constructs a timeout Escalation handler
         * 
         * Initialises the handler with the max number of days an approval request may wait till escalation
         * 
         * @param limitDays the waiting-time limit in days escalation occurs when the request has waited longer than the limit 
         */
        TimeoutEscalationHandler(int limitDays);
        /**
         * @brief Processes an approval request based on its waiting time
         * 
         * compares the request waiting time wiht the configured limit
         * 
         * if days is greater than limitDays the rrquest decision will be chnaged to escalted
         * 
         * otherwise the request is passed to the next handler in the chain if one is avaialble
         * 
         * @param request Reference to the approval request being processed
         */
        virtual void handle(ApprovalRequest& request);

};

#endif