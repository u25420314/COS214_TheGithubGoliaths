//Command : ConcreteCommand
//Memento : Caretaker

#ifndef ASSIGNTASKCOMMAND_H
#define ASSIGNTASKCOMMAND_H

#include "Task.h"
#include "Participant.h"
#include "TaskMemento.h"
#include "Command.h"

#include <vector>
#include <string>

using namespace std;

/**
 * @brief Assigns a task to a participant
 */
class AssignTaskCommand : public Command {
    private:
        Task* task; ///< The task to be assigned
        Participant* participant; ///< The participant to recieve the task
        TaskMemento* backup; ///< The backup in case of failure or undo

    public:
        /// @brief Assigns the command to the participant
        /// @param e The engine to be used
        /// @param t The task that is being assigned
        /// @param p The participant who is recieving the task
        AssignTaskCommand(WorkflowEngine* e, Task* t, Participant* p);

        /// @brief Saves a memento and assigns the task to the participant
        void execute() override;

        /// @brief restores the taskfrom the memento
        void undo() override;

        /// @brief deletes backup memento
        ~AssignTaskCommand();
};

#endif