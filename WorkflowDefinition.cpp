#include "WorkflowDefinition.h"
#include "WorkflowInstance.h"
#include "WorkNode.h"

//Constructor, stores name version and tree
WorkflowDefinition::WorkflowDefinition(const std::string& name, int version, WorkNode* root) {
    this->name = name;
    this->version = version;
    this->root = root;
}

//Destructor
WorkflowDefinition::~WorkflowDefinition() {
    delete root;
}

//Move constructor and move assignment take the other definitions tree and leave it empty so only one owner exists
WorkflowDefinition::WorkflowDefinition(WorkflowDefinition&& other) noexcept {
    name = other.name;
    version = other.version;
    root = other.root;
    other.root = nullptr;
}

//defines what = means for the class, copying is forbidden because it would double-delete and moving is allowed because it passes ownership
WorkflowDefinition& WorkflowDefinition::operator=(WorkflowDefinition&& other) noexcept {
    if (this != &other) {
        delete root;
        name = other.name;
        version = other.version;
        root = other.root;
        other.root = nullptr;
    }
    return *this;
}

//clones the template tree and wraps the copy in a workFlowInstance, it returns nullptr if there is no tree
WorkflowInstance* WorkflowDefinition::instantiate(const std::string& instanceId) const {
    if (root == nullptr) {
        return nullptr;
    }
    return new WorkflowInstance(instanceId, root->clone());
}

const std::string& WorkflowDefinition::getName() const {
    return name;
}

int WorkflowDefinition::getVersion() const {
    return version;
}

const WorkNode* WorkflowDefinition::getRoot() const {
    return root;
}
