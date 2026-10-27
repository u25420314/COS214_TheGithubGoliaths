#ifndef TIVIDY_WORKFLOWDIRECTOR_H
#define TIVIDY_WORKFLOWDIRECTOR_H

#include <functional>
#include <string>
#include <vector>

class WorkflowBuilder;

class WorkflowDirector {
public:
    explicit WorkflowDirector(const std::string& workflowName = "");
    virtual ~WorkflowDirector();

    WorkflowDirector& stage(const std::string& name, bool parallel = false);
    WorkflowDirector& task(const std::string& name, const std::string& role);
    WorkflowDirector& end();

    virtual void construct(WorkflowBuilder& builder);

private:
    std::string workflowName;
    std::vector<std::function<void(WorkflowBuilder&)> > steps;
};

#endif
