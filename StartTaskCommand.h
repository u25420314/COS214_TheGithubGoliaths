//Command : ConcreteCommand
//Memento : Caretaker

#ifndef STARTTASKCOMMAND_H
#define STARTTASKCOMMAND_H

#include "Task.h"
#include "Participant.h"
#include "TaskMemento.h"
#include "Command.h"

#include <vector>
#include <string>

using namespace std;

/**
 * @brief Completes a task.
 */
class StartTaskCommand : public Command {
    private:
        Task* task; ///< The task to be completes
        TaskMemento* backup; ///< The backup in case of failure or undo

    public:
        /// @brief Starts the task
        /// @param e The engine to be used
        /// @param t The task that is being completed
        StartTaskCommand(WorkflowEngine* e, Task* t);

        /// @brief Saves a memento and starts the task
        void execute() override;

        /// @brief restores the task from the memento
        void undo() override;

        /// @brief deletes backup memento
        ~StartTaskCommand();
};

#endif