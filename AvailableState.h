#ifndef AVAILABLESTATE_H
#define AVAILABLESTATE_H

#include "TaskState.h"

/// @brief Class representing the available state of a task
class AvailableState : public TaskState {
    public:
        /// @brief assigns the task to a participant thats available
        /// @param t the task to be assigned
        /// @param p the participant who is assigned the task
        void assign(Task* t, Participant* p) override;

        /// @brief cancels the task in the available state
        /// @param t the task to be canceled
        void cancel(Task* t) override;

        /// @brief gets the name of the available state
        /// @return the name of the available state
        std::string getName() const override;

        /// @brief clones the available state
        /// @return the available state object
        TaskState* clone() const override { return new AvailableState(*this); }
};

#endif