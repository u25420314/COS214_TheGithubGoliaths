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

class AssignTaskCommand : public Command {
    private:
        Task* task;
        Participant* participant;
        TaskMemento* backup;

    public:
        AssignTaskCommand(WorkflowEngine*, Task*, Participant*);

        void execute();
        void undo();

        ~AssignTaskCommand();
};

#endif ASSIGNTASKCOMMAND_H