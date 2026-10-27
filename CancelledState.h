#ifndef CANCELLEDSTATE_H
#define CANCELLEDSTATE_H

#include "TaskState.h"


class CancelledStae : public TaskState {
    public:
        std::string getName() const override;
        bool isTerminal() const override {return true; } //every action is refusedd since task was canclled
};

#endif