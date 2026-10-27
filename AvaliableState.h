#ifndef AVAILABLESTATE_H
#define AVAILABLESTATE_H

#include "TaskState.h"


class AvailableState : public TaskState {
    public:
        void assign(Task* t, Participant* p) override;
        void cancel(Task* t) override;
        std::string getName() const override;
};

#endif