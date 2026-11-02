#ifndef TIVIDY_STANDARDWORKFLOWBUILDER_H
#define TIVIDY_STANDARDWORKFLOWBUILDER_H

#include <string>
#include <vector>
#include "WorkflowBuilder.h"

class Task;
class WorkNode;
class Stage;

/// @brief Class representing a standard workflow builder for constructing workflow definitions
class StandardWorkflowBuilder : public WorkflowBuilder {
public:
    /// @brief Constructor for StandardWorkflowBuilder
    StandardWorkflowBuilder();
    /// @brief Destructor for StandardWorkflowBuilder
    ~StandardWorkflowBuilder() override;

    /// @brief Resets the builder to its initial state
    void reset() override;
    /// @brief sets the name of the workflow being built
    /// @param name the name of the workflow being built
    void setWorkflowName(const std::string& name) override;
    /// @brief begins a new stage in the workflow being built
    /// @param name the name of the stage being built
    /// @param parallel the parallel status of the stage being built
    void beginStage(const std::string& name, bool parallel) override;
    /// @brief adds a task to the current stage in the workflow being built
    /// @param name the name of the task being added
    /// @param role the role of the participant responsible for the task being added
    void addTask(const std::string& name, const std::string& role) override;
    /// @brief ends the current stage in the workflow being built
    void endStage() override;
    /// @brief adds an approval chain to the workflow being built
    /// @param chain the chain to be added to the workflow being built
    void withApprovalChain(ApprovalHandler* chain) override;
    /// @brief gives the definition a priority
    /// @param level the priority being set
    void withPriority(int level) override;

    /// @brief gets the results of the definition
    /// @return the results of the definition
    WorkflowDefinition getResult() override;
private:
    /// @brief The id of the definition
    /// @return 
    std::string newId();

    /// @brief The workflow name
    std::string workflowName;
    /// @brief The root of the definition and tasks
    Stage* root;
    /// @brief the open stages of the definition
    std::vector<Stage*> openStages;
    /// @brief the next ID
    int nextId;
    /// @brief the last WorkNode
    WorkNode* lastNode;

    /// @brief The last task
    Task* lastTask;
};

#endif
