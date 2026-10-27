#ifndef ROLEAPPROVER_H
#define ROLEAPPROVER_H
#include "ApprovalHandler.h"
#include <string>

class RoleApprover : public ApprovalHandler{
    private:
        std::string role;

    public:
        RoleApprover(const std::string& role);

        virtual void handle(ApprovalRequest& request);
};

#endif