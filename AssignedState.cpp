#include "AssignedState.h"
#include "InProgressState.h"
#include "CancelledState.h"
#include "Task.h"

std::string AssignedState::getName() const {
    return "Assigned";
}

void AssignedState::start(Task* t) {
    t->setState(new InProgressState());
}

void AssignedState::cancel(Task* t) {
    t->setState(new CancelledStae());
}

