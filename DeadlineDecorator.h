#ifndef DEADLINEDECORATOR_H
#define DEADLINEDECORATOR_H

#include "TaskDecorator.h"

class DeadlineDecorator : public TaskDecorator {
    protected:
        void doWork() override;
    private:
        int dueDays;
        int elapsed;
    public:
        DeadlineDecorator(WorkNode* wrapped, int dueDays);

        void advanceDays(int days); //adcance forwards
        int daysElapsed() const;
        int getDueDays() const;
        bool isOverdue() const;

        WorkNode* clone() const override;


};
#endif