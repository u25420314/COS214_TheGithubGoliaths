#ifndef PRIORITYDECORATOR_H
#define PRIORITYDECORATOR_H

#include "TaskDecorator.h"

class PriorityDecorator : public TaskDecorator {
    protected:
        void doWork() override; //logs the priority and then runs the wrapped work
    private:
        int level;
    public:
        PriorityDecorator(WorkNode* wrapped, int level);
        int getLevel() const;
        void setLevel(int level);


        WorkNode* clone() const override;


};
#endif