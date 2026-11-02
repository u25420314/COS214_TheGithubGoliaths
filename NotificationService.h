#pragma once
#include <iostream>
#include "WorkflowObserver.h"


/**
 * @class NotificationService
 * @author Simon Vogel
 * @brief Observer that prints notifications for important events relevant
 * to the User
 */
class NotificationService : public WorkflowObserver {
public:
    /**
     * @brief constructor that uses an ostream operator for outputting Notifications
     * @param ostream representing the output of the Notification 
     */
    
    explicit NotificationService(std::ostream& out = std::cout) : out(out) {}
    
    /**
     * @brief overriden from Observer, creates the Notification but isn't outputted
     * @param e The WorkflowEvent causing the notification to be necessary to push 
     */
    void update(const WorkflowEvent& e) override;

    /// @brief updates the notification service with a new workflow event
    /// @param e the event to be notified about
    void update(const WorkflowEvent& e) override;
private:
/**
 * @brief ostream that outputs the notifications
 */
    std::ostream& out;
};