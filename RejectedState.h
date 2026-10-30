#ifndef REJECTEDSTATE_H
#define REJECTEDSTATE_H

#include "TaskState.h"


class RejectedState : public TaskState {
    public:
        void assign(Task* t, Participant* p) override;
        std::string getName() const override;
        TaskState* clone() const override { return new RejectedState(*this); }
};

#endif