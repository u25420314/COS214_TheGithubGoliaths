//Facade : Facade
//Command : Client

#ifndef TIVIDYSYSTEM_H
#define TIVIDYSYSTEM_H

#include "WorkFlowEngine.h"
#include "CommandHistory.h"
#include "WorkflowDefinition.h"
#include "Participant.h"

#include <vector>
#include <string>

using namespace std;

class TividySystem {
    private:
        WorkFlowEngine engine;
        CommandHistory history;
        vector<WorkflowDefinition*> definitions;

    public:
        void registerDefinition(WorkflowDefinition*);
        string startWorkflow(string, WorkflowData);
        void assignTask(string, Participant*);
        void completeTask(string, Participant*);
        void undoLast();
        string getProgress(string);
};

#endif TIVIDYSYSTEM_H