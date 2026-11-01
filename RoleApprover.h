#ifndef ROLEAPPROVER_H
#define ROLEAPPROVER_H
#include "ApprovalHandler.h"
#include <string>

/// @brief Class representing a role-based approver in the approval chain
class RoleApprover : public ApprovalHandler{
    private:
        /// @brief the role that this approver is responsible for
        std::string role;

    public:
        /// @brief Constructor for RoleApprover
        /// @param role the role that this approver is responsible for
        RoleApprover(const std::string& role);

        /// @brief handles the approval request based on the role of the approver
        /// @param request the approval request to be handled
        virtual void handle(ApprovalRequest& request);
};

#endif