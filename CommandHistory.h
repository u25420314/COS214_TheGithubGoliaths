//Facade : Subclass System
//Command : Invoker

#ifndef COMMANDHISTORY_H
#define COMMANDHISTORY_H

#include "Command.h"

#include <stack>
#include <string>

using namespace std;

/**
 * @brief Runs commands and keeps them to be undone
 */
class CommandHistory {
    private:
        stack<Command*> done; ///< Command history of executed commands

    public:
        /// @brief creates empty history vector
        CommandHistory();

        /// @brief Executes a command and stores it incase it is to be undone
        /// @param Command to be executed
        void executeCommand(Command);

        /// @brief Undoes the most recently executed command
        void undo();

        /// @brief destroys yhe history vector
        ~CommandHistory();
};

#endif