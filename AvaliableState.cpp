#include "AvaliableState.h"
#include "AssignedState.h"
#include "CancelledState.h"
#include "Task.h"
#include "Participant.h"

std::string AvaliableSate::getName() const {
    return "Avaliable";
}

void AvaliableState::assign(Task* t, Participant* p){
    if (!p){
        throw std::invalid_argument("assign requires a participant");
        t->setAssignee(p);
        t->setState(new AssignedState());

    }
}

void AvaliableState::cancel(Task* t){
    t->setState(new CancelledStae());
}

