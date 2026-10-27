#ifndef ESCALATEDSTATE_H
#define ESCALATEDSTATE_H

#include "TaskState.h"


class EscalatedState : public TaskState {
    public:
        void assign(Task* t, Participant* p) override;
        std::string getName() const override;
};

#endif