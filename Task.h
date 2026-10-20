#ifndef TIVIDY_TASK_H
#define TIVIDY_TASK_H

#include <string>
#include "WorkNode.h"

class TaskState;
class Participant;
class AssignmentStrategy;
class ApprovalHandler;
class TaskMemento;

class Task : public WorkNode {
public:

    Task(const std::string& id, const std::string& name, const std::string& requiredRole);
    ~Task() override;

    void setState(TaskState* s);
    TaskState* getState() const;

    void assign(Participant* p);
    void start();
    void complete();
    void reject();
    void escalate();

    //Memento
    TaskMemento* createMemento() const;
    void restore(const TaskMemento* m);

    const std::string& getRequiredRole() const;
    Participant* getAssignee() const;

    //Strategy
    void setStrategy(AssignmentStrategy* s);
    AssignmentStrategy* getStrategy() const;

    //Chain of responsibility
    void setApprovalChain(ApprovalHandler* h);
    ApprovalHandler* getApprovalChain() const;

    void setOnReject(WorkNode* target);
    WorkNode* getOnReject() const;

    //Prototype
    WorkNode* clone() const override;

    WorkNodeIterator* createIterator() override;

    bool isComplete() const override;

protected:

    bool canStart() const override;

    void doWork() override;

private:
    std::string requiredRole; //what kind of person must do it
    TaskState* state;
    Participant* assignee; //who is doing the task
    AssignmentStrategy* strategy;
    ApprovalHandler* approvalChain;
    WorkNode* onReject;
};

#endif
