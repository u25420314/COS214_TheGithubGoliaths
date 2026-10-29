#pragma once
#include <memory>
#include <string>
#include <vector>
#include "WorkflowMediator.h"
#include "WorkflowObserver.h"
#include "Participant.h"

class Task;
class WorkNode;
class WorkflowData;
class WorkflowDefinition;
class WorkflowInstance;

/**
 * The Mediator (receives notify() from tasks and decides what happens next) and
 * the Subject of the Observer pattern (publishes events to attached observers).
 *
 * NOTE: WorkNode, Task and WorkflowInstance must declare "friend class WorkflowEngine;"
 * so the engine can read the private fields shown in the class diagram.
 */
class WorkflowEngine : public WorkflowMediator {
public:
    explicit WorkflowEngine(std::vector<Participant> participants = {});
    ~WorkflowEngine() override;

    /** Clone the definition into a new instance, wire its nodes to this engine, activate first tasks. */
    WorkflowInstance* createInstance(const WorkflowDefinition& d, const WorkflowData& data);

    /** Observer side. Observers are not owned. */
    void attach(WorkflowObserver* o);
    void detach(WorkflowObserver* o);
    void notifyObservers(const WorkflowEvent& e);

    /** Mediator side: a node reports an event ("completed", "rejected", "escalated", "undone"). */
    void notify(WorkNode* sender, const std::string& event) override;

    /** Assign every Available task in the instance whose dependencies are done. */
    void activateNext(WorkflowInstance& i);

    /** Choose a participant with the task's strategy and give the task to them. */
    void assign(Task* t);

private:
    WorkflowInstance* findInstanceOf(WorkNode* node);
    void unlockDownstream(WorkflowInstance& i);
    void publish(const std::string& type, WorkflowInstance& i, WorkNode* node);

    std::vector<std::unique_ptr<WorkflowInstance>> instances;
    std::vector<Participant> participants;
    std::vector<WorkflowObserver*> observers;
};