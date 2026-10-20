#pragma once

#include "WorkflowObserver.h" //damn now you dont work either
#include "WorkflowEvent.h"
#include <vector>

using namespace std;

class ProgressMonitor: public WorkflowObserver{
    private:
        int completedCount;
    public:
        void update(WorkflowEvent e);
        string report();
};