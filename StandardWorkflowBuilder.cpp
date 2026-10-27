#include "StandardWorkflowBuilder.h"
#include "Stage.h"
#include "Task.h"
#include <stdexcept>

//Constructor: starts with no root, no open stages and nextID = 1
StandardWorkflowBuilder::StandardWorkflowBuilder() {
    this->workflowName = "";
    this->root = nullptr;
    this->nextId = 1;
}

//Destructor deletes the root if it still owns one
StandardWorkflowBuilder::~StandardWorkflowBuilder() {
    delete root;
}

// makes n1, n2 and so on
std::string StandardWorkflowBuilder::newId() {
    return "n" + std::to_string(nextId++);
}

//deletes any half-built tree and clears everything
void StandardWorkflowBuilder::reset() {
    delete root;
    root = nullptr;
    openStages.clear();
    workflowName = "";
    nextId = 1;
}

//stores the name
void StandardWorkflowBuilder::setWorkflowName(const std::string& name) {
    workflowName = name;
}

//creates a stage, it becomes the root if none is open otherwise it is added to the top ipen stage, it is then pushed on the stack it throws if a finished root already exists
void StandardWorkflowBuilder::beginStage(const std::string& name, bool parallel) {
    if (openStages.empty() && root != nullptr) {
        throw std::logic_error("StandardWorkflowBuilder: the root stage is already finished");
    }

    Stage* stage = new Stage(newId(), name, parallel);
    if (openStages.empty()) {
        root = stage;
    } else {
        openStages.back()->add(stage);
    }
    openStages.push_back(stage);
}

//adds a new task to the top of the open stage, it throws if none is open
void StandardWorkflowBuilder::addTask(const std::string& name, const std::string& role) {
    if (openStages.empty()) {
        throw std::logic_error("StandardWorkflowBuilder: addTask() needs an open stage");
    }

    openStages.back()->add(new Task(newId(), name, role));
}

//pops the top stage, it throws if none is open
void StandardWorkflowBuilder::endStage() {
    if (openStages.empty()) {
        throw std::logic_error("StandardWorkflowBuilder: endStage() with no open stage");
    }
    openStages.pop_back();
}

//moves the tree into a WorkFLowDefinition, resets the builder and returns it, it throws if there is no root or a stage is still open
WorkflowDefinition StandardWorkflowBuilder::getResult() {
    if (root == nullptr || !openStages.empty()) {
        throw std::logic_error("StandardWorkflowBuilder: getResult() needs one finished root stage");
    }
    WorkflowDefinition result(workflowName, 1, root);
    root = nullptr;
    reset();
    return result;
}
