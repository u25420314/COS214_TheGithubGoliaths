#pragma once
#include <string>

class WorkNode;

/** Mediator interface: nodes talk to this instead of to each other. */
class WorkflowMediator {
public:
    virtual ~WorkflowMediator() = default;
    /** Called by a node when something happens to it (e.g. "completed"). */
    virtual void notify(WorkNode* sender, const std::string& event) = 0;
};