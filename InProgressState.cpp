#include "InProgressState.h"
#include "CompletedState.h"
#include "RejectedState.h"
#include "EscalatedState.h"
#include "CancelledState.h"
#include "Task.h"

void InProgressState::complete(Task* t){
    t->setState(new CompletedState());
}

std::string InProgressState::getName() const {
    return "InProgress";
}

void InProgressState::reject(Task* t){
    t->setState(new RejectedState());
}

void InProgressState::escalate(Task* t){
    t->setState(new EscalatedState());
}

void InProgressState::cancel(Task* t){
    t->setState(new CancelledStae());
}

