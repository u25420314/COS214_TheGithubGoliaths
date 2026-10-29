#include "StartWorkflowCommand.h"
#include "WorkflowEngine.h"
#include "WorkflowInstance.h"
#include "WorkflowDefinition.h"

#include <string>

using namespace std;

StartWorkflowCommand::StartWorkflowCommand(WorkflowEngine* e, WorkflowDefinition* d, WorkflowData data) : Command(e), definition(d), data(data), createdId("") {}

void StartWorkflowCommand::execute() {
    if(engine == nullptr || definition == nullptr) return;
    
    WorkflowInstance* instance = engine->createInstance(*definition, data);
    if(instance != nullptr) {
        createdId = instance->getId();
    }
}

void StartWorkflowCommand::undo() {
    return;
}

string StartWorkflowCommand::getCreatedId() const {
    return createdId;
}