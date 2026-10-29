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

WorkflowInstance* WorkflowEngine::createInstance(const WorkflowDefinition& def,
                                                 const WorkflowData& data) {
    // std::unique_ptr<WorkflowInstance> inst = def.instantiate();  // deep clone (Prototype)
    // inst->getData() = data;
    // for (WorkNode* n : collectNodes(*inst)) n->setMediator(this);

    // instances_.push_back(std::move(inst));
    // WorkflowInstance& ref = *instances_.back();
    // publish("workflow_started", ref, nullptr);
    // activateNext(ref);
    // return ref;

    return nullptr;
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

void WorkflowEngine::activateNext(WorkflowInstance& instance) {
    // for (WorkNode* n : collectNodes(instance)) {
    //     Task* t = n->asTask();
    //     if (t && stateName(t) == "Available" && t->canStart()) assign(t);
    // }

    return;
}

void WorkflowEngine::assign(Task* task) {
    // if (!task) return;
    // WorkflowInstance* inst = findInstanceOf(task);
    // AssignmentStrategy* strategy = task->getStrategy();
    // Participant* chosen = strategy ? strategy->choose(*task, participants_) : nullptr;

    // if (!chosen) {
    //     if (inst) publish("task_unassignable", *inst, task);
    //     return;
    // }
    // task->assign(chosen);
    // if (inst) publish("task_assigned", *inst, task);

    return;
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

// Undo knock-on: a task that was assigned only because its predecessor was complete
// must go back to Available if that predecessor is no longer complete.
void WorkflowEngine::unlockDownstream(WorkflowInstance& instance) {
    // for (WorkNode* n : collectNodes(instance)) {
    //     Task* t = dynamic_cast<Task*>(n);
    //     if (t && stateName(t) == "Assigned" && !t->canStart()) {
    //         t->unassign();
    //         publish("task_unassigned", instance, t);
    //     }
    // }

    return;
}

void WorkflowEngine::publish(const std::string& type, WorkflowInstance& i, WorkNode* node) {
    notifyObservers(WorkflowEvent(type, i.id, node ? node->id : ""));
}