#include "TividySystem.h"

#include <string>
#include <iostream>

using namespace std;

TividySystem::TividySystem() {}

// Adds definition to the definitions
void TividySystem::registerDefinition(WorkflowDefinition* d) {
    if(d == nullptr) {
        cout << "null definition cant be registered\n";
        return; 
    }

    definitions.push_back(d);
}

// Searches for definition and starts it with the data
string TividySystem::startWorkflow(string defName, WorkflowData data) {
    WorkflowDefinition* chosenDef = nullptr;
    
    for(WorkflowDefinition* definition : definitions) {
        if(definition->getName() == defName) {
            chosenDef = definition;
            break;
        }
    }

    if(chosenDef == nullptr) {
        return "Definition not found";
    }

    StartWorkflowCommand* command = new StartWorkflowCommand(&engine, chosenDef, data);
    history.executeCommand(command);
    
    return command->getCreatedID();
}

// Creates assign command and gives to history
void TividySystem::assignTask(string taskId, Participant* p) {
    if(p == nullptr) {
        cout << "null participant cant be assigned a task\n";
        return;
    }

    Task* task = engine.findTask(taskId);

    if(task == nullptr) {
        cout << "Task not found\n";
        return;
    }

    history.executeCommand(new AssignTaskCommand(&engine, task, p));
    
    return;
}

void TividySystem::undoLast() {
    history.undo();
}

string TividySystem::getProgress(string instanceId) {
    return engine.getProgress(instanceId);
}

TividySystem::~TividySystem() {
    for(WorkflowDefinition* definition : definitions) {
        delete d;
    }
}