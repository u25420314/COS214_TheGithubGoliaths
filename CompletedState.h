#ifndef COMPLETEDSTATE_H
#define COMPLETEDSTATE_H

#include "TaskState.h"


class CompletedState : public TaskState {
    public:
        
        std::string getName() const override;
        TaskState* clone() const override { return new CompletedState(*this); }
        bool isTerminal() const override { return true; }
        bool isComplete() const override { return true; }
};

#endif