#include "AssignTaskCommand.h"

using namespace std;

AssignTaskCommand::AssignTaskCommand(WorkflowEngine* e, Task* t, Participant* p) : Command(e), task(t), participant(p), backup(nullptr) {}

void AssignTaskCommand::execute() {
    if(task == nullptr || participant == nullptr) return;
    
    backup = task->createMemento();
    task->assign(participant);
}

void AssignTaskCommand::undo() {
    if(backup != nullptr) {
        task->restore(backup);
    }
}

AssignTaskCommand::~AssignTaskCommand() {
    delete backup;
}