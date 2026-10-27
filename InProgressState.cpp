#include "InProgressState.h"
#include "CompletedState.h"
#include "RejectedState.h"
#include "EscalatedState.h"
#include "CancelledState.h"
#include "Task.h"

void InProgressState::complete(Task* t){
    t-setstate(new CompletedState());
}

std::string InProgressState::getName() const {
    return "InProgress";
}

void InProgressState::reject(Task* t){
    t-setstate(new RejectedState());
}

void InProgressState::escalate(Task* t){
    t-setstate(new EscalatedState());
}

void InProgressState::cancel(Task* t){
    t-setstate(new CancelledStae());
}

