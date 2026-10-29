#include "WorkflowEngine.h"
#include <algorithm>
#include "AssignmentStrategy.h"
#include "AvailableState.h"
#include "Task.h"
#include "TaskState.h"
#include "WorkNode.h"
#include "WorkNodeIterator.h"
#include "WorkflowData.h"
#include "WorkflowDefinition.h"
#include "WorkflowInstance.h"

namespace {
// All nodes in an instance's tree, collected up front so we never change things mid-walk.
std::vector<WorkNode*> allNodes(WorkNode* root) {
    std::vector<WorkNode*> nodes;
    if (!root) return nodes;
    auto it = root->createIterator();
    for (it->first(); !it->isDone(); it->next()) nodes.push_back(it->current());
    return nodes;
}
}  // namespace

WorkflowEngine::WorkflowEngine(std::vector<Participant> participants)
    : participants(std::move(participants)) {}

WorkflowEngine::~WorkflowEngine() = default;

// ---- Mediator side --------------------------------------------------------

WorkflowInstance* WorkflowEngine::createInstance(const WorkflowDefinition& d,
                                                 const WorkflowData& data) {
    std::unique_ptr<WorkflowInstance> inst(d.instantiate());   // Prototype: deep copy
    inst->data = data;
    for (WorkNode* n : allNodes(inst->root.get())) n->mediator = this;

    instances.push_back(std::move(inst));
    WorkflowInstance* created = instances.back().get();
    publish("instance_created", *created, nullptr);
    activateNext(*created);
    return created;
}

void WorkflowEngine::notify(WorkNode* sender, const std::string& event) {
    WorkflowInstance* inst = findInstanceOf(sender);
    if (!inst) return;

    publish(event, *inst, sender);           // tell the observers first

    if (event == "completed" || event == "rejected") {
        activateNext(*inst);                 // something finished, or rejected work was re-routed
        if (event == "completed" && inst->isComplete())
            publish("instance_completed", *inst, nullptr);
    } else if (event == "undone") {
        unlockDownstream(*inst);
    }
}

void WorkflowEngine::activateNext(WorkflowInstance& i) {
    for (WorkNode* n : allNodes(i.root.get())) {
        Task* t = dynamic_cast<Task*>(n);
        if (t && t->state->getName() == "Available" && t->canStart()) assign(t);
    }
}

void WorkflowEngine::assign(Task* t) {
    if (!t) return;
    WorkflowInstance* inst = findInstanceOf(t);
    Participant* chosen = t->strategy ? t->strategy->choose(*t, participants) : nullptr;
    if (!chosen) {
        if (inst) publish("task_unassignable", *inst, t);
        return;
    }
    t->assign(chosen);
    if (inst) publish("task_assigned", *inst, t);
}

// ---- Observer side --------------------------------------------------------

void WorkflowEngine::attach(WorkflowObserver* o) {
    if (o && std::find(observers.begin(), observers.end(), o) == observers.end())
        observers.push_back(o);
}

void WorkflowEngine::detach(WorkflowObserver* o) {
    observers.erase(std::remove(observers.begin(), observers.end(), o), observers.end());
}

void WorkflowEngine::notifyObservers(const WorkflowEvent& e) {
    std::vector<WorkflowObserver*> snapshot = observers;   // observers may detach while we loop
    for (WorkflowObserver* o : snapshot)
        if (std::find(observers.begin(), observers.end(), o) != observers.end()) o->update(e);
}

// ---- helpers --------------------------------------------------------------

WorkflowInstance* WorkflowEngine::findInstanceOf(WorkNode* node) {
    if (!node) return nullptr;
    for (auto& inst : instances)
        for (WorkNode* n : allNodes(inst->root.get()))
            if (n == node) return inst.get();
    return nullptr;
}

// Undo knock-on: a task assigned only because its predecessor finished goes back to
// Available when that predecessor is no longer finished.
void WorkflowEngine::unlockDownstream(WorkflowInstance& i) {
    for (WorkNode* n : allNodes(i.root.get())) {
        Task* t = dynamic_cast<Task*>(n);
        if (t && t->state->getName() == "Assigned" && !t->canStart()) {
            t->setState(std::make_unique<AvailableState>());
            publish("task_unassigned", i, t);
        }
    }
}

void WorkflowEngine::publish(const std::string& type, WorkflowInstance& i, WorkNode* node) {
    notifyObservers(WorkflowEvent(type, i.id, node ? node->id : ""));
}