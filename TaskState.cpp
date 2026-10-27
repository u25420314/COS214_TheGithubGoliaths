#include "TaskState.h"

void TaskState::refuse(const std::string& action) const {
    throw InvalidTransitionError(action + " is not allowed in " + getName() + " state");
}

void TaskState::start(Task*){
    refuse("start");
}

void TaskState::assign(Task* t, Participant* p){
    refuse("assign");
}

void TaskState::complete(Task*){
    refuse("complete");
}

void TaskState::reject(Task*){
    refuse("reject");
}

void TaskState::escalate(Task*){
    refuse("escalate");
}

void TaskState::cancel(Task*){
    refuse("cancel");
}