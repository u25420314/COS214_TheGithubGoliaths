#ifndef TASKDECORATOR_H
#define TASKDECORATOR_H

#include "WorkNode.h"
#include "WorkflowMediator.h"


class TaskDecorator : public WorkNode {
    protected:
        void doWork() override;
        WorkNode* wrapped; //owned variable
    public:
        explicit TaskDecorator(WorkNode* wrapped);
        virtual ~TaskDecorator();

        std::vector<WorkNode*> getChildren() const override;
        WorkNodeIterator* createIterator() override;
        bool isComplete() const override;
        void setMediator(WorkflowMediator* m) override;
        void collectReady(std::vector<Task*>& out) override;

        bool canStart() const override;

        WorkNode* clone() const override = 0;
};

#endif