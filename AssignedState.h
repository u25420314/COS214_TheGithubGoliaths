#ifndef ASSIGNEDSTATE_H
#define ASSIGNEDSTATE_H

#include "TaskState.h"


class AssignedState : public TaskState {
    public:
        void start(Task* t) override;
        void cancel(Task* t) override;
        std::string getName() const override;
        TaskState* clone() const override { return new AssignedState(*this); }
};

#endif