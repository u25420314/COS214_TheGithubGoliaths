#include "WorkNode.h"

WorkNode::WorkNode(const std::string& id, const std::string& name)
    : id(id), name(name), mediator(nullptr) {}

WorkNode::~WorkNode() {}

// Template Method
void WorkNode::run() {
    if (canStart()) {
        doWork();
        finish();
    }
}

void WorkNode::add(WorkNode* child) {
    (void)child;
}

void WorkNode::remove(WorkNode* child) {
    (void)child;
}

std::vector<WorkNode*> WorkNode::getChildren() const {
    return std::vector<WorkNode*>();
}

const std::string& WorkNode::getId() const {
    return id;
}

const std::string& WorkNode::getName() const {
    return name;
}

void WorkNode::setMediator(WorkflowMediator* m) {
    mediator = m;
}

WorkflowMediator* WorkNode::getMediator() const {
    return mediator;
}


bool WorkNode::canStart() const {
    return true;
}

void WorkNode::finish() {}
