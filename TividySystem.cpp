#include "TividySystem.h"

#include <string>

using namespace std;

void TividySystem::registerDefinition(WorkflowDefinition* definition) {
    return;
}

string TividySystem::startWorkflow(string defName, WorkflowData data) {
    return "";
}

void TividySystem::assignTask(string taskId, Participant* p) {
    return;
}

void TividySystem::undoLast() {
    return;
}

string TividySystem::getProgress(string instanceId) {
    return instanceId;
}