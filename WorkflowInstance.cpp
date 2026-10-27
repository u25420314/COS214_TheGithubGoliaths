#include "WorkflowInstance.h"
#include "WorkNode.h"

//Cosntructor: stores the id and tree and starts with status "Running"
WorkflowInstance::WorkflowInstance(const std::string& id, WorkNode* root) {
    this->id = id;
    this->status = "Running";
    this->root = root;
}

//Destructor deletes its tree
WorkflowInstance::~WorkflowInstance() {
    delete root;
}

const std::string& WorkflowInstance::getId() const {
    return id;
}

const std::string& WorkflowInstance::getStatus() const {
    return status;
}

void WorkflowInstance::setStatus(const std::string& s) {
    status = s;
}

WorkNode* WorkflowInstance::getRoot() const {
    return root;
}

WorkflowData& WorkflowInstance::getData() {
    return data;
}

const WorkflowData& WorkflowInstance::getData() const {
    return data;
}

//is true if the tree exists and is complete
bool WorkflowInstance::isComplete() const {
    return root != nullptr && root->isComplete();
}
