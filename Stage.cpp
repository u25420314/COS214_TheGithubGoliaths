#include "Stage.h"
#include "DepthFirstIterator.h"

Stage::Stage(const std::string& id, const std::string& name, bool parallel)
    : WorkNode(id, name), parallel(parallel) {}

Stage::~Stage() {
    for (size_t i = 0; i < children.size(); i++) {
        delete children[i];
    }
    children.clear();
}

void Stage::add(WorkNode* child) {
    if (child == nullptr) {
        return;
    }
    child->setMediator(mediator);
    children.push_back(child);
}

void Stage::remove(WorkNode* child) {
    for (size_t i = 0; i < children.size(); i++) {
        if (children[i] == child) {
            children.erase(children.begin() + i);
            return;
        }
    }
}

std::vector<WorkNode*> Stage::getChildren() const {
    return children;
}

void Stage::setMediator(WorkflowMediator* m) {
    WorkNode::setMediator(m);
    for (size_t i = 0; i < children.size(); i++) {
        children[i]->setMediator(m);
    }
}

bool Stage::isParallel() const {
    return parallel;
}

// Parallel: every unfinished child is ready.
// Sequential: only the first unfinished child is ready.
std::vector<WorkNode*> Stage::getReadyChildren() const {
    std::vector<WorkNode*> ready;
    for (size_t i = 0; i < children.size(); i++) {
        if (!children[i]->isComplete()) {
            ready.push_back(children[i]);
            if (!parallel) {
                break;
            }
        }
    }
    return ready;
}

// Prototype: deep copy, so the clone gets its own copy of every child.
WorkNode* Stage::clone() const {
    Stage* copy = new Stage(id, name, parallel);
    for (size_t i = 0; i < children.size(); i++) {
        copy->add(children[i]->clone());
    }
    copy->mediator = mediator;
    return copy;
}

WorkNodeIterator* Stage::createIterator() {
    return new DepthFirstIterator(this);
}

bool Stage::isComplete() const {
    for (size_t i = 0; i < children.size(); i++) {
        if (!children[i]->isComplete()) {
            return false;
        }
    }
    return true;
}

bool Stage::canStart() const {
    return WorkNode::canStart() && !isComplete();
}

void Stage::doWork() {
    std::vector<WorkNode*> ready = getReadyChildren();
    for (size_t i = 0; i < ready.size(); i++) {
        ready[i]->run();
    }
}
