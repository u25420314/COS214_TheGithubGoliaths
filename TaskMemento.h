//Memento : Memento

#ifndef TASKMEMENTO_H
#define TASKMEMENTO_H

#include "WorklowData.h"

#include <string>

using namespace std;

class TaskMemento {
    private:
        string stateName;
        string assigneeid;
        WorkflowData payload;
        string timestamp;
};

#endif TASKMEMENTO_H