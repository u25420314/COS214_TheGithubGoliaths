#ifndef TIVIDY_TASK_H
#define TIVIDY_TASK_H

#include <string>
#include "WorkNode.h"

/// @brief forward declaration of TaskState
class TaskState;
/// @brief forward declaration of Participant
class Participant;
/// @brief forward declaration of AssignmentStrategy
class AssignmentStrategy;
/// @brief forward declaration of ApprovalHandler
class ApprovalHandler;
/// @brief forward declaration of TaskMemento
class TaskMemento;

/// @brief The class of the task used in the composite
class Task : public WorkNode {
public:
    /// @brief constructor of Task
    /// @param id the id of the Task
    /// @param name the name of the Task
    /// @param requiredRole The role needed to approve the task
    Task(const std::string& id, const std::string& name, const std::string& requiredRole);

    /// @brief The destructor of the Task
    ~Task() override;

    /// @brief sets the state of the task
    /// @param s the state to place task in
    void setState(TaskState* s);

    /// @brief gets the state of the task
    /// @return the state of the task
    TaskState* getState() const;

    /// @brief gives the state a participant to run it
    /// @param p the new participant of the state
    void assign(Participant* p);

    /// @brief gives the task a participant
    /// @param p the participant to use
    void setAssignee(Participant* p);

    /// @brief starts the task
    void start();

    /// @brief completes the task
    void complete();

    /// @brief rejects the task
    void reject();

    /// @brief escalates the task
    void escalate();

    /// @brief cancels the task
    void cancel();

    /// @brief stores the internal state of the task
    /// @return the created memento
    TaskMemento* createMemento() const;
    /// @brief restores the task to the memento
    /// @param m the task to be restored
    void restore(const TaskMemento* m);

    /// @brief gets the required role
    /// @return the required role
    const std::string& getRequiredRole() const;

    /// @brief gets the assignee of the participant
    /// @return the assignee of the participant
    Participant* getAssignee() const;

    /// @brief sets the strategy of the task of who to assign it to
    /// @param s the strategy to use
    void setStrategy(AssignmentStrategy* s);

    /// @brief returns the strategy that the task uses
    /// @return the strategy that the task uses
    AssignmentStrategy* getStrategy() const;

    /// @brief sets the approval chain
    /// @param h chain that the task uses
    void setApprovalChain(ApprovalHandler* h);

    /// @brief gets the approval chain
    /// @return the approval chain
    ApprovalHandler* getApprovalChain() const;

    /// @brief rejects the given WorkNode
    /// @param target the worknode being rejected
    void setOnReject(WorkNode* target);

    /// @brief rejects the given WorkNode id
    /// @param targetId the id to be rejected
    void setOnRejectId(const std::string& targetId);

    /// @brief gets the rejected Id
    /// @return the rejected Id
    const std::string& getOnRejectId() const;

    /// @brief clones the Task
    /// @return the cloned task
    WorkNode* clone() const override;

    /// @brief creates an iterator to traverse the task
    /// @return the iterator
    WorkNodeIterator* createIterator() override;

    /// @brief checks if the task is complete
    /// @return returns true if it is complete, or false otherwise
    bool isComplete() const override;

    /// @brief checks if the task can start
    /// @return returns true if it can start, or false otherwise 
    bool canStart() const override;

    /// @brief get the classes object
    /// @return the classes object
    Task* asTask() override { return this; }

    /// @brief collects the ready tasks
    /// @param out the vector of tasks to be assigned
    void collectReady(std::vector<Task*>& out) override;
protected:
    /// @brief does the tasks work for template
    void doWork() override;

private:
    /// @brief the required role needed to approve the task
    std::string requiredRole;
    /// @brief the state of the task
    TaskState* state;
    /// @brief the participant assigned to the task
    Participant* assignee;
    /// @brief the strategy to assign the task to a participant
    AssignmentStrategy* strategy;
    /// @brief the chain of responsibility that the task goes into
    ApprovalHandler* approvalChain;
    /// @brief It says where a tasks work should go if its rejected
    std::string onRejectId;
};

#endif
