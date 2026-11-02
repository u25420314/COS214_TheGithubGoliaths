#ifndef TIVIDY_WORKFLOWDEFINITION_H
#define TIVIDY_WORKFLOWDEFINITION_H

#include <string>

class WorkNode;
class WorkflowInstance;

/// @brief Class for WorkflowDefinition
class WorkflowDefinition {
public:
    /// @brief constructor of WorkflowDefinition
    /// @param name the name of the definition
    /// @param version the version of the definition
    /// @param root the root of the stage
    WorkflowDefinition(const std::string& name = "", int version = 1, WorkNode* root = nullptr);

    /// @brief destructor of the WorkflowDefinition
    ~WorkflowDefinition();

    /// @brief Move constructor and move assignment take the other definitions tree and leave it empty so only one owner exists
    /// @param other the other WorkflowDefinition
    WorkflowDefinition(WorkflowDefinition&& other) noexcept;

    /// @brief defines what = means for the class, copying is forbidden because it would double-delete and moving is allowed because it passes ownership
    /// @param other the other WorkflowDefinition
    /// @return returns the workflowDefinition
    WorkflowDefinition& operator=(WorkflowDefinition&& other) noexcept;

    WorkflowDefinition(const WorkflowDefinition&) = delete;

    WorkflowDefinition& operator=(const WorkflowDefinition&) = delete;

    /// @brief instantiates the definition
    /// @param instanceId the id of the instance
    /// @return the WorkflowInstance
    WorkflowInstance* instantiate(const std::string& instanceId) const;

    /// @brief gets the name
    /// @return the name
    const std::string& getName() const;

    /// @brief gets the version
    /// @return the version
    int getVersion() const;

    /// @brief gets the root
    /// @return the root
    const WorkNode* getRoot() const;

private:
    /// @brief the name of the definition
    std::string name;
    /// @brief the version
    int version;
    /// @brief what WorkNode it starts on
    WorkNode* root;
};

#endif
