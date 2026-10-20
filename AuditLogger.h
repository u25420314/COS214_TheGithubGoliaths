#pragma once

#include "WorkflowObserver.h" //damn now you dont work either
#include "WorkflowEvent.h"
#include <vector>

using namespace std;

class AuditLogger: public WorkflowObserver{
    private:
        vector<string> entries;
    public:
        void update(WorkflowEvent e);
};