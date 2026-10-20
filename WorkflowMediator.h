#pragma once

//This thing also doesn't exist but fukken ayt
#include "WorkNode.h"

using namespace std;

class WorkflowMediator{
    public:
        void notify(WorkNode sender, string event);
};