#ifndef TIVIDY_WORKFLOWINSTANCE_H
#define TIVIDY_WORKFLOWINSTANCE_H

#include <string>
#include "WorkflowData.h"

class WorkNode;

class WorkflowInstance {
public:

    WorkflowInstance(const std::string& id, WorkNode* root);
    ~WorkflowInstance();

    WorkflowInstance(const WorkflowInstance&) = delete;
    WorkflowInstance& operator=(const WorkflowInstance&) = delete;

    const std::string& getId() const;
    const std::string& getStatus() const;
    void setStatus(const std::string& s);

    WorkNode* getRoot() const;
    WorkflowData& getData();
    const WorkflowData& getData() const;

    bool isComplete() const;

private:
    std::string id;
    std::string status;
    WorkflowData data;
    WorkNode* root;
};

#endif
