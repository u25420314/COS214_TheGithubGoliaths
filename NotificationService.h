#pragma once
#include <iostream>
#include "WorkflowObserver.h"

/** Observer: prints a notification for the events a user would care about. */
class NotificationService : public WorkflowObserver {
public:
    explicit NotificationService(std::ostream& out = std::cout) : out(out) {}
    void update(const WorkflowEvent& e) override;

private:
    std::ostream& out;
};