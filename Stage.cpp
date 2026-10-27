#include "Stage.h"
#include "DepthFirstIterator.h"

//constructor
Stage::Stage(const std::string& id, const std::string& name, bool parallel) : WorkNode(id, name) {
    this->parallel = parallel;
}

//destructor, deletes every child ownership is recursive
Stage::~Stage() {
    for (size_t i = 0; i < children.size(); i++) {
        delete children[i];
    }
    children.clear();
}

//ignores a null child, itself or one already added, otherwise it gives the child the stages's mediator and stores it
void Stage::add(WorkNode* child) {
    if (child == nullptr || child == this) {
        return;
    }
    for (size_t i = 0; i < children.size(); i++) {
        if (children[i] == child) {
            return;
        }
    }
    child->setMediator(mediator);
    children.push_back(child);
}

//removes it from the list without deleting it
void Stage::remove(WorkNode* child) {
    for (size_t i = 0; i < children.size(); i++) {
        if (children[i] == child) {
            children.erase(children.begin() + i);
            return;
        }
    }
}

//returns copy of the child list
std::vector<WorkNode*> Stage::getChildren() const {
    return children;
}


//sets it on the stage and passses it down to every child
void Stage::setMediator(WorkflowMediator* m) {
    WorkNode::setMediator(m);
    for (size_t i = 0; i < children.size(); i++) {
        children[i]->setMediator(m);
    }
}

//returns the flag
bool Stage::isParallel() const {
    return parallel;
}

//goes through the children and collects those that arent complete. a parallel stage keeps all of them sequential stage stops after the first
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

//Prototype: makes a new stage and adds a clone of every child so the copy is deep
WorkNode* Stage::clone() const {
    Stage* copy = new Stage(id, name, parallel);
    for (size_t i = 0; i < children.size(); i++) {
        copy->add(children[i]->clone());
    }
    copy->setMediator(mediator);
    return copy;
}

//Iterator: returns a depthFirstIterator for the whole subtree
WorkNodeIterator* Stage::createIterator() {
    return new DepthFirstIterator(this);
}

//is true only when every child is complete 
bool Stage::isComplete() const {
    for (size_t i = 0; i < children.size(); i++) {
        if (!children[i]->isComplete()) {
            return false;
        }
    }
    return true;
}

//is true if the stage isnt complete
bool Stage::canStart() const {
    return WorkNode::canStart() && !isComplete();
}

//runs run() on every child
void Stage::doWork() {
    std::vector<WorkNode*> ready = getReadyChildren();
    for (size_t i = 0; i < ready.size(); i++) {
        ready[i]->run();
    }
}
