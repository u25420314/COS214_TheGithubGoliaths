#ifndef TIVIDY_WORKNODE_H
#define TIVIDY_WORKNODE_H

#include <string>
#include <vector>

class WorkflowMediator;
class WorkNodeIterator;
class Task;

class WorkNode {
public:

    WorkNode(const std::string& id, const std::string& name);
    virtual ~WorkNode();
    WorkNode(const WorkNode&) = delete;
    WorkNode& operator=(const WorkNode&) = delete;

    void run();
    virtual void add(WorkNode* child);
    virtual void remove(WorkNode* child);
    virtual std::vector<WorkNode*> getChildren() const;

    virtual WorkNode* clone() const = 0;
    virtual WorkNodeIterator* createIterator() = 0;
    virtual bool isComplete() const = 0;

    const std::string& getId() const;
    const std::string& getName() const;
    virtual void setMediator(WorkflowMediator* m);
    WorkflowMediator* getMediator() const;
    virtual bool canStart() const;
    virtual Task* asTask() { return nullptr; }

protected:
    virtual void doWork() = 0;
    virtual void finish();

    std::string id;
    std::string name;
    WorkflowMediator* mediator;
};

#endif
