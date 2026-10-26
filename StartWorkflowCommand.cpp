#include "StartWorkflowCommand.h"

#include <string>

using namespace std;

StartWorkflowCommand::StartWorkflowCommand(WorkflowEngine* e, WorkflowDefinition* d, WorkflowData data) : Command(e), definition(d), data(data), createdId("") {}

void StartWorkflowCommand::execute() {
    if(engine == nullptr || definition == nullptr) return;
    
    WorkflowInstance* instance = engine->createInstance(definition, data);
    if(instance != nullptr) {
        createdId = instance->getId()
    }
}

void StartWorkflowCommand::undo() {

}

string StartWorkflowCommand::getCreatedId() const {
    return createdId;
}

StartWorkflowCommand::~StartWorkflowCommand() {
    delete backup;
}