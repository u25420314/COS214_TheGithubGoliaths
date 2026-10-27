#ifndef AVALIABLESTATE_H
#define AVALIABLESATE_H

#include "TaskState.h"


class AvaliableSate : public TaskState {
    public:
        void assign(Task* t, Participant* p) override;
        void cancel(Task* t) override;
        std::string getName() const override;
};

#endif