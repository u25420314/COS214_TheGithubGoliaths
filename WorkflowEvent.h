#pragma once

#include <string>

using namespace std;

class WorkflowEvent{
    public:
        string type;
        string instanceId; 
        string nodeId;
        string time;
}; //Damn the only file that works :D