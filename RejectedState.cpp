#include "RejectedState.h"
#include "AssignedState.h"
#include "Task.h"
#include "Participant.h"

std::string RejectedState::getName() const { \
    return "Rejected";
}

void RejectedState::assign(Task* t, Participant* p){
    if (!p) throw std::invalid_argument("assign requires a participant");
    t->setAssignee(p);
    t->setState(new AssignedState());
}



