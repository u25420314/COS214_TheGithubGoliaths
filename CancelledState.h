#ifndef CANCELLEDSTATE_H
#define CANCELLEDSTATE_H

#include "TaskState.h"


class CancelledState : public TaskState {
    public:
        bool isTerminal() const override {return true; } //every action is refusedd since task was canclled
        std::string getName() const override;
        TaskState* clone() const override { return new CancelledState(*this); }
};

#endif