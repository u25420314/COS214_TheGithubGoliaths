#include "Task.h"
#include "TaskState.h"
#include "AvailableState.h"
#include "TaskMemento.h"
#include "DepthFirstIterator.h"
#include "WorkflowMediator.h"
#include "ApprovalHandler.h"
#include "ApprovalRequest.h"

//Constructor stores role, creates AvailableState as starting state and sets everything else to null
Task::Task(const std::string& id, const std::string& name, const std::string& requiredRole) : WorkNode(id, name) {
    this->requiredRole = requiredRole;
    this->state = new AvailableState();
    this->assignee = nullptr;
    this->strategy = nullptr;
    this->approvalChain = nullptr;
    this->onRejectId = "";
}

//Destructor
Task::~Task() {
    delete state;
}

//swaps the new state and deletes the old one
void Task::setState(TaskState* s) {
    if (s == state) {
        return;
    }
    TaskState* old = state;
    state = s;
    delete old;
}

//returns state
TaskState* Task::getState() const {
    return state;
}


//forward to the current state, the state decides what happens (it calls setAssignee)
void Task::assign(Participant* p) {
    if (state != nullptr) {
        state->assign(this, p);
    }
}

//stores who is doing the task, called by the states
void Task::setAssignee(Participant* p) {
    assignee = p;
}

//forward to the current state, the state decides what happens
void Task::start() {
    if (state != nullptr) {
        state->start(this);
        if(mediator) mediator->notify(this, "started");
    }
}

//forward to the current state, the state decides what happens
void Task::complete() {
    if(state == nullptr) return;

    if(approvalChain != nullptr) {
        ApprovalRequest request(this, 0);
        approvalChain->handle(request);

        if(request.getDecision() == "Escalated") {
            state->escalate(this);

            if(mediator) mediator->notify(this, "escalated");
            return;
        }

        if(request.getDecision() != "Approved") {
            throw InvalidTransitionError("Complete needs approval: " + getName() + " still needs approval");
        }
    }

    state->complete(this);
    if(mediator) mediator->notify(this, "completed");
}

void Task::collectReady(std::vector<Task*>& out) {
    if(state != nullptr && state->getName() == "Available") {
        out.push_back(this);
    }
}

//forward to the current state, the state decides what happens
void Task::reject() {
    if (state != nullptr) {
        state->reject(this);
        if(mediator) mediator->notify(this, "rejected");
    }
}

//forward to the current state, the state decides what happens
void Task::escalate() {
    if (state != nullptr) {
        state->escalate(this);
        if(mediator) mediator->notify(this, "escalated");
    }
}

//forward to the current state, the state decides what happens
void Task::cancel() {
    if (state != nullptr) {
        state->cancel(this);
        if(mediator) mediator->notify(this, "cancelled");
    }
}


//Saves a copy of the state and assignee
TaskMemento* Task::createMemento() const {
    TaskState* stateCopy = (state != nullptr) ? state->clone() : nullptr;
    return new TaskMemento(stateCopy, assignee);
}

//puts the saved state and the saved assignee back
void Task::restore(const TaskMemento* m) {
    if (m == nullptr) {
        return;
    }
    if (m->getState() != nullptr) {
        setState(m->getState()->clone());
    }
    assignee = m->getAssignee();
    if(mediator) mediator->notify(this, "undone");
}

const std::string& Task::getRequiredRole() const {
    return requiredRole;
}

Participant* Task::getAssignee() const {
    return assignee;
}

//Strategy pattern
void Task::setStrategy(AssignmentStrategy* s) {
    strategy = s;
}

//Strategy pattern
AssignmentStrategy* Task::getStrategy() const {
    return strategy;
}

//Chain of Responsibility
void Task::setApprovalChain(ApprovalHandler* h) {
    approvalChain = h;
}

//Chain of Responsibility
ApprovalHandler* Task::getApprovalChain() const {
    return approvalChain;
}

//saves the other tasks id
void Task::setOnReject(WorkNode* target) {
    onRejectId = (target != nullptr) ? target->getId() : std::string();
}
//saves id directly
void Task::setOnRejectId(const std::string& targetId) {
    onRejectId = targetId;
}

//returns the id
const std::string& Task::getOnRejectId() const {
    return onRejectId;
}

//Prototype step, makes a new task with the same id, name and roles, copies the shared pointers and onRejectId, and copies the mediator, the new task has a fresh state and no assignee
WorkNode* Task::clone() const {
    Task* copy = new Task(id, name, requiredRole);
    copy->strategy = strategy;
    copy->approvalChain = approvalChain;
    copy->onRejectId = onRejectId;
    copy->mediator = mediator;
    return copy;
}

//returns a depthFirstIterator on itself which visits just this one node
WorkNodeIterator* Task::createIterator() {
    return new DepthFirstIterator(this);
}

//asks the state if its complete
bool Task::isComplete() const {
    return state != nullptr && state->isComplete();
}

//is true if there is an assignee and a state
bool Task::canStart() const {
    return WorkNode::canStart() && assignee != nullptr && state != nullptr;
}

//call start so the state machine does the work
void Task::doWork() {
    start();
}
