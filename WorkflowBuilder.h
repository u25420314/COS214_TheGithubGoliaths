#ifndef TIVIDY_WORKFLOWBUILDER_H
#define TIVIDY_WORKFLOWBUILDER_H

#include <string>
#include "WorkflowDefinition.h"

class ApprovalHandler;

class WorkflowBuilder {
public:
    virtual ~WorkflowBuilder() {}

    virtual void reset() = 0;
    virtual void setWorkflowName(const std::string& name) = 0;

    virtual void beginStage(const std::string& name, bool parallel) = 0;

    virtual void addTask(const std::string& name, const std::string& role) = 0;
    virtual void endStage() = 0;

    virtual void withApprovalChain(ApprovalHandler *chain) = 0;

    virtual void withPriority(int level) = 0;

    virtual WorkflowDefinition getResult() = 0;
};

#endif
