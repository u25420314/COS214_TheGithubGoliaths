//Command : ConcreteCommand
//Memento : Caretaker

#ifndef COMPLETETASKCOMMAND_H
#define COMPLETETASKCOMMAND_H

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
class CompleteTaskCommand : public Command {
    private:
        Task* task; ///< The task to be completes
        Participant* participant; ///< The participant who holds the task
        TaskMemento* backup; ///< The backup in case of failure or undo

    public:
        /// @brief Completes the task held by the participant
        /// @param e The engine to be used
        /// @param t The task that is being completed
        /// @param p The participant who holds the task
        CompleteTaskCommand(WorkflowEngine* e, Task* t, Participant* p);

        /// @brief Saves a memento and completes the task
        void execute() override;

        /// @brief restores the task from the memento
        void undo() override;

        /// @brief deletes backup memento
        ~CompleteTaskCommand();
};

#endif