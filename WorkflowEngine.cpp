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

#include<string>

namespace {
    std::vector<WorkNode*> allNodes(WorkNode* root) {
        std::vector<WorkNode*> nodes;
        if (!root) return nodes;
        auto it = root->createIterator();
        for (it->first(); !it->isDone(); it->next()) nodes.push_back(it->current());
        return nodes;
    }

    std::vector<WorkNode*> collectNodes(WorkflowInstance& instance) {
        return allNodes(instance.getRoot());
    }

    std::string stateName(Task* t) { return t->getState()->getName(); }
}  // namespace

WorkflowEngine::WorkflowEngine(std::vector<Participant> participants)
    : participants(std::move(participants)) {}

WorkflowEngine::~WorkflowEngine() = default;

WorkflowInstance* WorkflowEngine::createInstance(const WorkflowDefinition& def, const WorkflowData& data) {
    std::unique_ptr<WorkflowInstance> inst(def.instantiate("INST-" + std::to_string(instances.size() + 1)));  // deep clone (Prototype)
    inst->getData() = data;
    for (WorkNode* n : collectNodes(*inst)) n->setMediator(this);

    instances.push_back(std::move(inst));
    WorkflowInstance& ref = *instances.back();
    publish("workflow_started", ref, nullptr);
    activateNext(ref);
    return &ref;
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
    for (WorkNode* n : collectNodes(instance)) {
        Task* t = n->asTask();
        if (t && stateName(t) == "Available" && t->canStart()) assign(t);
    }

    return;
}

void WorkflowEngine::assign(Task* task) {
    if (!task) return;
    WorkflowInstance* inst = findInstanceOf(task);
    AssignmentStrategy* strategy = task->getStrategy();
    
    std::vector<Participant*> pool;
    for(Participant& p : participants) pool.push_back(&p);
    Participant* chosen = strategy ? strategy->choose(*task, pool) : nullptr;

    if (!chosen) {
        if (inst) publish("task_unassignable", *inst, task);
        return;
    }
    task->assign(chosen);
    if (inst) publish("task_assigned", *inst, task);

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

//Helpers: 

WorkflowInstance* WorkflowEngine::findInstanceOf(WorkNode* node) {
    if (!node) return nullptr;
    for (auto& inst : instances)
        for (WorkNode* n : collectNodes(*inst))
            if (n == node) return inst.get();
    return nullptr;
}

// Undo knock-on: a task that was assigned only because its predecessor was complete
// must go back to Available if that predecessor is no longer complete.
void WorkflowEngine::unlockDownstream(WorkflowInstance& instance) {
    for (WorkNode* n : collectNodes(instance)) {
        Task* t = dynamic_cast<Task*>(n);
        if (t && stateName(t) == "Assigned" && !t->canStart()) {
            
            t->setAssignee(nullptr);
            t->setState(new AvailableState());

            publish("task_unassigned", instance, t);
        }
    }

    return;
}

void WorkflowEngine::publish(const std::string& type, WorkflowInstance& i, WorkNode* node) {
    notifyObservers(WorkflowEvent(type, i.getId(), node ? node->getId() : ""));
}

Task* WorkflowEngine::findTask(const std::string& taskId) {
    for(auto& inst : instances) {
        for(WorkNode* n : collectNodes(*inst)) {
            Task* t = n->asTask();
            if(t && t->getId() == taskId) return t;
        }
    }

    return nullptr;
}