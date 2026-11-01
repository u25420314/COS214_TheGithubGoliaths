#ifndef TIVIDY_WORKFLOWBUILDER_H
#define TIVIDY_WORKFLOWBUILDER_H

#include <string>
#include "WorkflowDefinition.h"

/// @brief forward declaration of ApprovalHandler
class ApprovalHandler;

/// @brief Class for WorkflowBuilder
class WorkflowBuilder {
public:
    /// @brief destructor for workflowBuilder
    virtual ~WorkflowBuilder() {}

    /// @brief reset the builder
    virtual void reset() = 0;
    /// @brief set the name of the definition
    /// @param name the name of the definition
    virtual void setWorkflowName(const std::string& name) = 0;

    /// @brief begins the construction stage
    /// @param name the name of the stage
    /// @param parallel determines if its parallel or sequential
    virtual void beginStage(const std::string& name, bool parallel) = 0;

    /// @brief constructs a task to the Builder
    /// @param name the name of the task
    /// @param role the role needed to complete the task
    virtual void addTask(const std::string& name, const std::string& role) = 0;
    
    /// @brief marks the end of the construction of a stage
    virtual void endStage() = 0;

    /// @brief creates an approval chain for the definition
    /// @param chain the chain of responsibility
    virtual void withApprovalChain(ApprovalHandler *chain) = 0;

    /// @brief gives the defition a priority
    /// @param level the level of priority
    virtual void withPriority(int level) = 0;

    /// @brief gets the result of the definition
    /// @return the definition
    virtual WorkflowDefinition getResult() = 0;
};

#endif
