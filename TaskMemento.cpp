#include "TaskMemento.h"

#include <string>

using namespace std;

TaskMemento::TaskMemento(TaskState* state, Participant* assignee) {
    this->state = state;
    this->assignee = assignee;
}

TaskState* TaskMemento::getState() const {
    return state;
}

Participant* TaskMemento::getAssignee() const {
    return assignee;
}

TaskMemento::~TaskMemento() {
    delete state;
}