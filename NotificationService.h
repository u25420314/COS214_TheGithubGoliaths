#pragma once
#include <iostream>
#include "WorkflowObserver.h"

/// @brief class representing a notification service that observes workflow events
class NotificationService : public WorkflowObserver {
public:
    /// @brief constructor for the notification service
    /// @param out the output stream where notifications will be sent
    explicit NotificationService(std::ostream& out = std::cout) : out(out) {}

    /// @brief updates the notification service with a new workflow event
    /// @param e the event to be notified about
    void update(const WorkflowEvent& e) override;
private:
    /// @brief the output stream where notifications will be sent
    std::ostream& out;
};