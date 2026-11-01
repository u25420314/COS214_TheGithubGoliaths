#include "CompleteTaskCommand.h"

using namespace std;

CompleteTaskCommand::CompleteTaskCommand(WorkflowEngine* e, Task* t, Participant* p) : Command(e), task(t), participant(p), backup(nullptr) {}

void CompleteTaskCommand::execute() {
    if(task == nullptr) return;
    
    delete backup;
    backup = task->createMemento();
    task->complete();
}

void CompleteTaskCommand::undo() {
    if(backup != nullptr) {
        task->restore(backup);
    }
}

CompleteTaskCommand::~CompleteTaskCommand() {
    delete backup;
}