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
        CommandHistory();

        void executeCommand(Command);
        void undo();

        ~CommandHistory();
};

#endif COMMANDHISTORY_H