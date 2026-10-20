//Facade : Subclass System
//Command : Invoker

#ifndef COMMANDHISTORY_H
#define COMMANDHISTORY_H

#include "Command.h"

#include <stack>
#include <string>

using namespace std;

class CommandHistory {
    private:
        stack<Command*> done;

    public:
        void executeCommand(Command*);
        void undo();
};

#endif COMMANDHISTORY_H