#ifndef ROLEAPPROVER_H
#define ROLEAPPROVER_H
#include "ApprovalHandler.h"
#include <string>
/**
 * @class RoleApprover
 * @brief Approves requests based on participant roles.
 * @details ConcreteHandler in the Chain of Responsibility pattern.
 */
class RoleApprover : public ApprovalHandler{
    private:
        std::string role;

    public:
        /**
     * @brief Constructs an approver with a required role.
     * @param role Role needed for approval.
     */
        RoleApprover(const std::string& role);
        /**
     * @brief Approves the request if the role matches,
     *        otherwise forwards it to the next handler.
     * @param request Approval request to process.
     */
        virtual void handle(ApprovalRequest& request);
};

#endif