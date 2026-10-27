#ifndef TIVIDY_WORKFLOWDEFINITION_H
#define TIVIDY_WORKFLOWDEFINITION_H

#include <string>

class WorkNode;
class WorkflowInstance;

class WorkflowDefinition {
public:

    WorkflowDefinition(const std::string& name = "", int version = 1, WorkNode* root = nullptr);
    ~WorkflowDefinition();

    WorkflowDefinition(WorkflowDefinition&& other) noexcept;
    WorkflowDefinition& operator=(WorkflowDefinition&& other) noexcept;
    WorkflowDefinition(const WorkflowDefinition&) = delete;
    WorkflowDefinition& operator=(const WorkflowDefinition&) = delete;

    WorkflowInstance* instantiate(const std::string& instanceId) const;

    const std::string& getName() const;
    int getVersion() const;
    const WorkNode* getRoot() const;

private:
    std::string name;
    int version;
    WorkNode* root;
};

#endif
