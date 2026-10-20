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

class CompleteTaskCommand : public Command {
    private:
        Task* task;
        Participant* participant;
        TaskMemento* backup;

    public:
        void execute();
        void undo();
};

#endif COMPLETETASKCOMMAND_H