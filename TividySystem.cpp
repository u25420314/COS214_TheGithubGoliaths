#include "TividySystem.h"

#include <string>
#include <iostream>

using namespace std;

TividySystem::TividySystem(vector<Participant> participants) : engine(participants) {
    engine.attach(&auditLogger);
}

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
    
    return command->getCreatedId();
}

// Creates assign command and gives to history
void TividySystem::assignTask(string taskId, Participant* p) {
    if(p == nullptr) return;

    Task* task = engine.findTask(taskId);
    if(task == nullptr) return;

    history.executeCommand(new AssignTaskCommand(&engine, task, p));
}

void TividySystem::startTask(string taskId) {
    Task* task = engine.findTask(taskId);
    if(task == nullptr) return;
    history.executeCommand(new StartTaskCommand(&engine, task));
}

void TividySystem::completeTask(string taskId) {
    Task* task = engine.findTask(taskId);
    if(task == nullptr) return;
    history.executeCommand(new CompleteTaskCommand(&engine, task, task->getAssignee()));
}

void TividySystem::rejectTask(string taskId) {
    Task* task = engine.findTask(taskId);
    if(task == nullptr) return;
    history.executeCommand(new RejectTaskCommand(&engine, task));
}

vector<string> TividySystem::getHistory() const {
    return auditLogger.getEntries();
}

void TividySystem::undoLast() {
    history.undo();
}

TividySystem::~TividySystem() {
    for(WorkflowDefinition* definition : definitions) {
        delete definition;
    }
}