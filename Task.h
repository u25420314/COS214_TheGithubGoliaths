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
    void setAssignee(Participant* p);
    void start();
    void complete();
    void reject();
    void escalate();
    void cancel();

    TaskMemento* createMemento() const;
    void restore(const TaskMemento* m);

    const std::string& getRequiredRole() const;
    Participant* getAssignee() const;

    void setStrategy(AssignmentStrategy* s);
    AssignmentStrategy* getStrategy() const;

    void setApprovalChain(ApprovalHandler* h);
    ApprovalHandler* getApprovalChain() const;

    void setOnReject(WorkNode* target);
    void setOnRejectId(const std::string& targetId);
    const std::string& getOnRejectId() const;

    WorkNode* clone() const override;

    WorkNodeIterator* createIterator() override;

    bool isComplete() const override;

    bool canStart() const override;
    Task* asTask() override { return this; }

protected:

    void doWork() override;

private:
    std::string requiredRole;
    TaskState* state;
    Participant* assignee;
    AssignmentStrategy* strategy;
    ApprovalHandler* approvalChain;
    std::string onRejectId;
};

#endif
