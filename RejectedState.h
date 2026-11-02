#ifndef REJECTEDSTATE_H
#define REJECTEDSTATE_H

#include "TaskState.h"

/// @brief Class representing the rejected state of a task
class RejectedState : public TaskState {
    public:
        /// @brief assigns the task to a participant in the rejected state
        /// @param t the task to be assigned
        /// @param p the participant that is assigned the task
        void assign(Task* t, Participant* p) override;

        /// @brief gets the name of the rejected state
        /// @return the name of the rejected state
        std::string getName() const override;

        /// @brief clones the rejected state
        /// @return the cloned rejected state object
        TaskState* clone() const override { return new RejectedState(*this); }
};

#endif