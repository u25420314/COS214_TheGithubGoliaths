#ifndef TIVIDY_WORKFLOWDIRECTOR_H
#define TIVIDY_WORKFLOWDIRECTOR_H

#include <functional>
#include <string>
#include <vector>

/// @brief forward declaration of WorkflowBuilder
class WorkflowBuilder;

/// @brief class for WorkflowDirector
class WorkflowDirector {
public:
    /// @brief constructor for WorkflowDirector
    /// @param workflowName the name of the workflow
    explicit WorkflowDirector(const std::string& workflowName = "");
    /// @brief destructor for WorkflowDirector
    virtual ~WorkflowDirector();

    /// @brief creates the stage
    /// @param name the name of the stage
    /// @param parallel if its parralell or not
    /// @return the director
    WorkflowDirector& stage(const std::string& name, bool parallel = false);

    /// @brief creates the task
    /// @param name the name of the task
    /// @param role the role
    /// @return the director
    WorkflowDirector& task(const std::string& name, const std::string& role);

    /// @brief end the creation
    /// @return the director
    WorkflowDirector& end();

    /// @brief constructs the definition
    /// @param builder the builder will be used
    virtual void construct(WorkflowBuilder& builder);

private:
    /// @brief name of the workflow
    std::string workflowName;
    /// @brief the steps required to build the workflow
    std::vector<std::function<void(WorkflowBuilder&)> > steps;
};

#endif
