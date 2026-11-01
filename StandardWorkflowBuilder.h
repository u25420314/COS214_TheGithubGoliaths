#ifndef TIVIDY_STANDARDWORKFLOWBUILDER_H
#define TIVIDY_STANDARDWORKFLOWBUILDER_H

#include <string>
#include <vector>
#include "WorkflowBuilder.h"

class Task;
class WorkNode;
class Stage;

class StandardWorkflowBuilder : public WorkflowBuilder {
public:
    StandardWorkflowBuilder();
    ~StandardWorkflowBuilder() override;

    void reset() override;
    void setWorkflowName(const std::string& name) override;
    void beginStage(const std::string& name, bool parallel) override;
    void addTask(const std::string& name, const std::string& role) override;
    void endStage() override;
    void withApprovalChain(ApprovalHandler* chain) override;
    void withPriority(int level) override;

    WorkflowDefinition getResult() override;

private:
    std::string newId();

    std::string workflowName;
    Stage* root;
    std::vector<Stage*> openStages;
    int nextId;
    WorkNode* lastNode;

    Task* lastTask;
};

#endif
