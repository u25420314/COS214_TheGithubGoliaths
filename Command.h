//Command : Command

#ifndef COMMAND_H
#define COMMAND_H

#include "WorkflowEngine.h"

using namespace std;

class Command {
    protected:
        WorkflowEngine* engine;

    public:
        virtual void execute() = 0;
        virtual void undo() = 0;
};

#endif COMMAND_H