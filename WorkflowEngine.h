//Yah no NONE of these files exist but sure what the hell 
#include "Workflowinstance.h"
#include "Participant.h"
#include "WorkflowObserver.h"
#include "WorkflowData.h"
#include "WorkflowInstance.h"
#include "WorkflowEvent.h"
#include "WorkNode.h"
#include "Task.h"
#include <vector>

#pragma once

using namespace std;

class WorkflowEngine{
    private:
        vector<Workflowinstance> instances;
        vector<Participant> participants;
        vector<WorkflowObserver> observers;
    public: 
        WorkflowInstance createInstance(WorkflowData d);
        void attach(WorkflowObserver o);
        void notify(WorkNode sender, string event);
        void detach(WorkflowObserver o);
        void notifyObservers(WorkflowEvent e);
        void activateNext(WorkflowInstance i);
        void assign(Task t);
};