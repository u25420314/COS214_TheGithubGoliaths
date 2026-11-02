//Command : Command

#ifndef COMMAND_H
#define COMMAND_H

#include "WorkflowEngine.h"

using namespace std;

/**
 * @brief Base case for every action in the workflow system
 */
class Command {
    protected:
        WorkflowEngine* engine; ///< The reciever that carries out the work

    public:
        /// @brief Creates the command to work with the given engine
        /// @param e The engine the command must work with
        Command(WorkflowEngine* e) : engine(e) {}
        virtual ~Command() = default;
        /// @brief Performs the action with invalid actions being refused
        virtual void execute() = 0;

        /// @brief The undo's the action performed by executed
        virtual void undo() = 0;

        /// @brief ConcreteCommands are deleted through the virtual destructor
        
};

#endif