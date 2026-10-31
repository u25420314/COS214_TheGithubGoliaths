#include "StartTaskCommand.h"

using namespace std;

StartTaskCommand::StartTaskCommand(WorkflowEngine* e, Task* t) : Command(e), task(t), backup(nullptr) {}

void StartTaskCommand::execute() {
    if(task == nullptr) return;
    
    delete backup;
    backup = task->createMemento();
    task->run();
}

void StartTaskCommand::undo() {
    if(backup != nullptr) {
        task->restore(backup);
    }
}

StartTaskCommand::~StartTaskCommand() {
    delete backup;
}