#include "EscalatedState.h"
#include "AssignedState.h"
#include "Task.h"
#include "Participant.h"

std::string EscalatedState::getName() const {
    return "Escalated"
}

void EscalatedState::assign(Task* t, Participant* p){
    if (!p)
    {
        throw std::invalid_argument("Assign requires a particpant")
    }

    t->setAssignee(p);
    t->setState(new AssignedState())
    
}

