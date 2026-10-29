#pragma once
#include <string>

class WorkNode;

/** <<Interface>> Nodes report to this instead of talking to each other. */
class WorkflowMediator {
public:
    virtual ~WorkflowMediator() = default;
    virtual void notify(WorkNode* sender, const std::string& event) = 0;
};