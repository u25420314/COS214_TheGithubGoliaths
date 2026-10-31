#ifndef PRIORITYDECORATOR_H
#define PRIORITYDECORATOR_H

#include "TaskDecorator.h"

class PriorityDecorator : public TaskDecorator {
    private:
        int level;
    public:
        PriorityDecorator(WorkNode* wrapped, int level);
        int getLevel() const;
        void setLevel(int level);
        void collectReady(std::vector<Task*>& out) override;

        WorkNode* clone() const override;


};
#endif