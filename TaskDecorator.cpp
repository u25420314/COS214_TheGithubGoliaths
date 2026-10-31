#include "TaskDecorator.h"
#include "DepthFirstIterator.h"

TaskDecorator::TaskDecorator(WorkNode* wrapped) : WorkNode(wrapped ? wrapped->getId() : "", wrapped ? wrapped->getName() : ""), wrapped(wrapped) {}

TaskDecorator::~TaskDecorator() {
    delete wrapped;
}

std::vector<WorkNode*> TaskDecorator::getChildren() const {
    std::vector<WorkNode*> kids;

    if(wrapped != nullptr) kids.push_back(wrapped);

    return kids;
}

WorkNodeIterator* TaskDecorator::createIterator() {
    return new DepthFirstIterator(this);
}

bool TaskDecorator::isComplete() const {
    return wrapped != nullptr && wrapped->isComplete();
}

bool TaskDecorator::canStart() const {
    return wrapped != nullptr && wrapped->canStart();
}

void TaskDecorator::setMediator(WorkflowMediator* m) {
    WorkNode::setMediator(m);
    if(wrapped != nullptr) wrapped->setMediator(m);
}

void TaskDecorator::collectReady(std::vector<Task*>& out) {
    if(wrapped != nullptr) wrapped->collectReady(out);
}

void TaskDecorator::doWork() {
    if(wrapped != nullptr) wrapped->run();
}