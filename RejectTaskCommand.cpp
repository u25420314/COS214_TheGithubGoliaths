#include "RejectTaskCommand.h"

using namespace std;

RejectTaskCommand::RejectTaskCommand(WorkflowEngine* e, Task* t) : Command(e), task(t), backup(nullptr) {}

void RejectTaskCommand::execute() {
    if(task == nullptr) return;
    
    delete backup;
    backup = task->createMemento();
    task->reject();
}

void RejectTaskCommand::undo() {
    if(backup != nullptr) {
        task->restore(backup);
    }
}

RejectTaskCommand::~RejectTaskCommand() {
    delete backup;
}