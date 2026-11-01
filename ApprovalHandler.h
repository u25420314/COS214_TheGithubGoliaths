#ifndef APPROVALHANDLER_H
#define APPROVALHANDLER_H

class ApprovalRequest;

/**
 * @class ApprovalHandler
 * @brief Abstract hander in the chain of respsonsibility design pattern.
 * 
 * Defines an interface for processing approval requests and maintaina a pointer to the next handler in the chain
 */
class ApprovalHandler{
    protected:
    /**
     * @brief Pointer to the next handler in the chain
     * 
     * used to forward approval requests when the current handler does not handle them
     */
        ApprovalHandler* next;
    public:
    /**
     * @brief Constructs an ApprovalHandler
     * 
     * initiales the next handerpointer to null
     */
        ApprovalHandler();
        /**
         * @brief virtual destrcuctor for ApprovalHandler
         */
        virtual ~ApprovalHandler();
        /**
         * @brief sets the next handler in the chain of responsibility
         * 
         * links the current handler to another handler
         * allowing approval requests to be passed down the chain of responsibility
         * 
         * @param handler pointer to the next ApprovalHandler
         * @return Pointer to the next handler that was assigned as next.
         */
        ApprovalHandler* setNext(ApprovalHandler* handler);
        /**
         * @brief Processes an approval request
         * 
         * concrete handlers implement this method to determine whether they can process the request,
         * if they cannot it may be moved forward to the enxt hander in the chain
         * 
         * @param request Reference to the ApprovalRequest being processed.
         */
        virtual void handle(ApprovalRequest& request) = 0;

};

#endif