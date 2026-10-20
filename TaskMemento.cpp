#include "TaskMemento.h"

#include <string>

using namespace std;

TaskMemento::TaskMemento(string stateName, string assigneeid, WorkflowData payload, string timestamp) {
    this->stateName = stateName;
    this->assigneeid = assigneeid;
    this->payload = payload;
    this->timestamp = timestamp;
}