#ifndef TASKDECORATOR_H
#define TASKDECORATOR_H

#include "WorkNode.h"


class TaskDecorator : public WorkNode {
    protected:
        bool canStart() const override;
        void doWork() override;
        WorkNode* wrapped; //owned variable
    public:
        explicit TaskDecorator(WorkNode* wrapped);
        virtual ~TaskDecorator();

        WorkNode* clone() const override = 0;
};

#endif