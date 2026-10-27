#ifndef APPROVALHANDLER_H
#define APPROVALHANDLER_H

class ApprovalRequest;


class ApprovalHandler{
    protected:
        ApprovalHandler* next;
    public:
        ApprovalHandler();
        virtual ~ApprovalHandler();
        ApprovalHandler* setNext(ApprovalHandler* handler);
        virtual void handle(ApprovalRequest& request) = 0;

};

#endif