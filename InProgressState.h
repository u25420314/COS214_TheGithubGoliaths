#ifndef INPROGRESSSTATE_H
#define INPROGRESSSTATE_H

#include "TaskState.h"


class InProgressState : public TaskState {
    public:
        void complete(Task* t) override;
        void reject(Task* t) override;
        void escalate(Task* t) override;
        void cancel(Task* t) override;
        std::string getName() const override;
        TaskState* clone() const override { return new InProgressState(*this); }
};

#endif