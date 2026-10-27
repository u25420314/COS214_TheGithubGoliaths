#include "WorkflowEngine.h"
#include "AssignmentStrategy.h"
#include "Task.h"
#include "TaskState.h"
#include "WorkNode.h"
#include "WorkNodeIterator.h"
#include "WorkflowData.h"
#include "WorkflowDefinition.h"
#include "WorkflowInstance.h"

namespace {
// Every node in the instance's tree (collected first so we never mutate while iterating).
std::vector<WorkNode*> collectNodes(WorkflowInstance& instance) {
    std::vector<WorkNode*> nodes;
    WorkNode* root = instance.getRoot();
    if (!root) return nodes;
    auto it = root->createIterator();
    for (it->first(); !it->isDone(); it->next()) nodes.push_back(it->current());
    return nodes;
}

// Isolated here so there is ONE place to change if Task's state API differs.
std::string stateName(Task* t) { return t->getState()->getName(); }
}  // namespace

WorkflowEngine::WorkflowEngine(std::vector<Participant> participants)
    : participants_(std::move(participants)) {}

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
    if (!inst) return;  // node doesn't belong to any instance we manage

    publish("task_" + event, *inst, sender);

    if (event == "completed" || event == "approved" || event == "rejected") {
        activateNext(*inst);   // something finished, or rejected work was re-routed
        checkFinished(*inst);
    } else if (event == "undone") {
        unlockDownstream(*inst);
        finished_.erase(inst->getId());
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

// ---- private helpers ------------------------------------------------------

WorkflowInstance* WorkflowEngine::findInstanceOf(WorkNode* node) {
    if (!node) return nullptr;
    for (auto& inst : instances_)
        for (WorkNode* n : collectNodes(*inst))
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

void WorkflowEngine::checkFinished(WorkflowInstance& instance) {
    if (instance.isComplete() && finished_.insert(instance.getId()).second)
        publish("workflow_completed", instance, nullptr);
}

void WorkflowEngine::publish(const std::string& type, const WorkflowInstance& instance,
                             const WorkNode* node) {
    notifyObservers(WorkflowEvent(type, instance.getId(), node ? node->getId() : ""));
}