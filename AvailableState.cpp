#include "AvailableState.h"
#include "AssignedState.h"
#include "CancelledState.h"
#include "Task.h"
#include "Participant.h"

std::string AvailableState::getName() const {
    return "Avaliable";
}

void AvailableState::assign(Task* t, Participant* p){
    if (!p){
        throw std::invalid_argument("assign requires a participant");
        t->setAssignee(p);
        t->setState(new AssignedState());

    }
}

void AvailableState::cancel(Task* t){
    t->setState(new CancelledStae());
}

