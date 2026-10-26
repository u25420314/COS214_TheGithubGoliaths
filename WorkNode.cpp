#include "WorkNode.h"

//Composite: Component

//Constructor stores id, name and sets mediator
WorkNode::WorkNode(const std::string& id, const std::string& name) {
    this->id = id;
    this->name = name;
    this->mediator = nullptr;
}

//Destructor
WorkNode::~WorkNode() {}

//template method
void WorkNode::run() {
    if (canStart()) {
        doWork();
        finish();
    }
}

//nothing by default as as leaf has no children, Stage overrides
void WorkNode::add(WorkNode* child) {
    (void)child;
}

//nothing by default as as leaf has no children, Stage overrides
void WorkNode::remove(WorkNode* child) {
    (void)child;
}

//returns empty list by default(which makes Task look like a leaf to iterator)
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

//returns true by default and subclasses add their own conditions
bool WorkNode::canStart() const {
    return true;
}

void WorkNode::finish() {}
