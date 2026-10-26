#include "CommandHistory.h"
#include "Command.h"

#include <string>
#include <vector>

using namespace std;

CommandHistory::CommandHistory() {}

void CommandHistory::executeCommand(Command* c) {
    if(c == nullptr) return;

    c->execute();
    
    done.push_back(c);
}

void CommandHistory::undo() {
    if(done.empty()) return;

    Command* last = done.back();
    done.pop_back();
    last->undo();

    delete last;
}

CommandHistory::~CommandHistory() {
    for(Command* c : done) {
        delete c;
    }
}