//Command : ConcreteCommand

#ifndef STARTWORKFLOWCOMMAND_H
#define STARTWORKFLOWCOMMAND_H

#include "WorkFlowDefinition.h"
#include "WorkflowData.h"
#include "Command.h"

#include <vector>
#include <string>

using namespace std;

class WorkflowDefinition

class StartWorkflowCommand : public Command {
    private:
        WorkflowDefinition* definition;
        WorkflowData data;
        string createdId

    public:
        StartWorkflowCommand(WorkflowEngine*, WorkflowDefinition*, WorkflowData)

        void execute();
        void undo();

        string getCreatedId() const;
};

#endif STARTWORKFLOWCOMMAND_H