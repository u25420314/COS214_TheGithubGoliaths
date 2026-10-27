#pragma once
#include <memory>
#include <set>
#include <string>
#include <vector>
#include "WorkflowMediator.h"
#include "WorkflowSubject.h"
#include "Participant.h"

class Task;
class WorkNode;
class WorkflowData;
class WorkflowDefinition;
class WorkflowInstance;

/**
 * The Mediator and the Subject. Tasks report to notify(); the engine decides what
 * happens next (activate follow-on tasks, detect completion, handle undo) and
 * publishes events to attached observers.
 */
class WorkflowEngine : public WorkflowMediator, public WorkflowSubject {
public:
    explicit WorkflowEngine(std::vector<Participant> participants = {});
    ~WorkflowEngine() override;

    /** Instantiate a definition, wire every node to this engine, activate the first tasks. */
    WorkflowInstance* createInstance(const WorkflowDefinition& def, const WorkflowData& data);

    /** Mediator entry point. Events: "completed", "rejected", "escalated", "undone", ... */
    void notify(WorkNode* sender, const std::string& event) override;

    /** Assign every Available task in the instance whose dependencies are done. */
    void activateNext(WorkflowInstance& instance);

    /** Pick a participant via the task's AssignmentStrategy and assign. */
    void assign(Task* task);

    void addParticipant(const Participant& p) { participants_.push_back(p); }
    std::vector<Participant>& getParticipants() { return participants_; }
    const std::vector<std::unique_ptr<WorkflowInstance>>& getInstances() const { return instances_; }

private:
    WorkflowInstance* findInstanceOf(WorkNode* node);
    void unlockDownstream(WorkflowInstance& instance);
    void checkFinished(WorkflowInstance& instance);
    void publish(const std::string& type, const WorkflowInstance& instance, const WorkNode* node);

    std::vector<std::unique_ptr<WorkflowInstance>> instances_;
    std::vector<Participant> participants_;
    std::set<std::string> finished_;   // ids of instances already announced as complete
};