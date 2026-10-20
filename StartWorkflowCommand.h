//Command : ConcreteCommand

#ifndef STARTWORKFLOWCOMMAND_H
#define STARTWORKFLOWCOMMAND_H

#include "WorkFlowDefinition.h"
#include "WorkflowData.h"
#include "Command.h"

#include <vector>
#include <string>

using namespace std;

class StartWorkflowCommand : public Command {
    private:
        WorkflowDefinition* definition;
        WorkflowData data;
        string createdId

    public:
        void execute();
        void undo();
};

#endif STARTWORKFLOWCOMMAND_H