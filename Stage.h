#ifndef TIVIDY_STAGE_H
#define TIVIDY_STAGE_H

#include <string>
#include <vector>
#include "WorkNode.h"

class Stage : public WorkNode {
public:

    Stage(const std::string& id, const std::string& name, bool parallel = false);
    ~Stage() override;

    void add(WorkNode* child) override;
    void remove(WorkNode* child) override;
    std::vector<WorkNode*> getChildren() const override;
    void setMediator(WorkflowMediator* m) override;

    bool isParallel() const;
    std::vector<WorkNode*> getReadyChildren() const;

    WorkNode* clone() const override;
    WorkNodeIterator* createIterator() override;
    bool isComplete() const override;

protected:

    bool canStart() const override;
    void doWork() override;

private:
    std::vector<WorkNode*> children;
    bool parallel;
};

#endif
