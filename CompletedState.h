#ifndef COMPLETEDSTATE_H
#define COMPLETEDSTATE_H

#include "TaskState.h"


class CompletedState : public TaskState {
    public:
        
        std::string getName() const override;
        bool isTerminal() const override { return true; }
};

#endif