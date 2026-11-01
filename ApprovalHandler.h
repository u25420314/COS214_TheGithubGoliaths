#ifndef APPROVALHANDLER_H
#define APPROVALHANDLER_H

/// @brief Forward declaration of the ApprovalRequest class
class ApprovalRequest;

/// @brief Abstract base class for handling approval requests
class ApprovalHandler{
    protected:
        /// @brief Pointer to the next handler in the chain
        ApprovalHandler* next;
    public:
        /// @brief Constructor for ApprovalHandler
        ApprovalHandler();

        /// @brief Destructor for ApprovalHandler
        virtual ~ApprovalHandler();

        /// @brief Sets the next handler in the chain
        /// @param handler Pointer to the next handler
        ApprovalHandler* setNext(ApprovalHandler* handler);

        /// @brief Handles the approval request
        /// @param request Reference to the approval request
        virtual void handle(ApprovalRequest& request) = 0;

};

#endif