#include "Task.h"
#include "TaskState.h"
#include "TaskMemento.h"
#include "DepthFirstIterator.h"

Task::Task(const std::string& id, const std::string& name, const std::string& requiredRole) : WorkNode(id, name), requiredRole(requiredRole),state(nullptr), assignee(nullptr), strategy(nullptr), approvalChain(nullptr), onReject(nullptr) {}

Task::~Task() {}

// State
void Task::setState(TaskState* s) {
    state = s;
}

TaskState* Task::getState() const {
    return state;
}

void Task::assign(Participant* p) {
    assignee = p;
}

void Task::start() {
    if (state != nullptr) {
        state->start(this);
    }
}

void Task::complete() {
    if (state != nullptr) {
        state->complete(this);
    }
}

void Task::reject() {
    if (state != nullptr) {
        state->reject(this);
    }
}

void Task::escalate() {
    if (state != nullptr) {
        state->escalate(this);
    }
}

// Memento: snapshot of what can change on a task (state + assignee).
TaskMemento* Task::createMemento() const {
    return new TaskMemento(state, assignee);
}

void Task::restore(const TaskMemento* m) {
    if (m == nullptr) {
        return;
    }
    state = m->getState();
    assignee = m->getAssignee();
}

const std::string& Task::getRequiredRole() const {
    return requiredRole;
}

Participant* Task::getAssignee() const {
    return assignee;
}

// Strategy
void Task::setStrategy(AssignmentStrategy* s) {
    strategy = s;
}

AssignmentStrategy* Task::getStrategy() const {
    return strategy;
}

// Chain of Responsibility
void Task::setApprovalChain(ApprovalHandler* h) {
    approvalChain = h;
}

ApprovalHandler* Task::getApprovalChain() const {
    return approvalChain;
}

void Task::setOnReject(WorkNode* target) {
    onReject = target;
}

WorkNode* Task::getOnReject() const {
    return onReject;
}

// Prototype: copies the configuration of the task. The assignee is NOT copied, because a fresh copy has not been assigned to anyone yet.
WorkNode* Task::clone() const {
    Task* copy = new Task(id, name, requiredRole);
    copy->state = state;
    copy->strategy = strategy;
    copy->approvalChain = approvalChain;
    copy->onReject = onReject;
    copy->mediator = mediator;
    return copy;
}

WorkNodeIterator* Task::createIterator() {
    return new DepthFirstIterator(this);
}

bool Task::isComplete() const {
    return state != nullptr && state->isComplete();
}

// A task can only start once someone has been assigned and it has a state.
bool Task::canStart() const {
    return WorkNode::canStart() && assignee != nullptr && state != nullptr;
}

// Running a task means starting it. The state decides what that does.
void Task::doWork() {
    start();
}
