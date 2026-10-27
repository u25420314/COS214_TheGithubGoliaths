#include "TaskDecorator.h"
#include <stdexcept>

namespace {
    WorkNode* requireNode(WorkNode* n){
        if(!n){
            throw std::invalidArgument("Task decorator requires a node to wrap");
            return n;
        }
    }

    std::string idOf(WorkNode* n){
        return requireNode(n)->getId();
    }

    std::string nameOf(Worknode* n){
        return requireNode(n)->getName();
    }
}

TaskDecorator::TaskDecorator(WorkNode* wrapped)
    : WorkNode(idOf(wrapped), nameOf(wrapped)), this->wrapped(wrapped){

}

TaskDecorator::~TaskDecorator() {
    delete this->wrapped;
}

bool TaskDecorator::canStart() const {
    return this->wrapped->canStart();
}

void TaskDecorator::doWork(){
    this->wrapped->doWork();
}

