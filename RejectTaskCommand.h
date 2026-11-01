//Command : ConcreteCommand
//Memento : Caretaker

#ifndef REJECTTASKCOMMAND_H
#define REJECTTASKCOMMAND_H

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
class RejectTaskCommand : public Command {
    private:
        Task* task; ///< The task to be rejected
        TaskMemento* backup; ///< The backup in case of failure or undo

    public:
        /// @brief rejects the task
        /// @param e The engine to be used
        /// @param t The task that is being completed
        RejectTaskCommand(WorkflowEngine* e, Task* t);

        /// @brief Saves a memento and rejects the task
        void execute() override;

        /// @brief restores the task from the memento
        void undo() override;

        /// @brief deletes backup memento
        ~RejectTaskCommand();
};

#endif