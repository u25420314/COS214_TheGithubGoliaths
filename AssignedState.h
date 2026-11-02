#ifndef ASSIGNEDSTATE_H
#define ASSIGNEDSTATE_H

#include "TaskState.h"

/// @brief Class representing the assigned state of a task
class AssignedState : public TaskState {
    public:
        /// @brief starts the task in the assigned state
        /// @param t the task to be started
        void start(Task* t) override;

        /// @brief assigns the task to a participant in the assigned state
        /// @param t the task to be assigned
        void cancel(Task* t) override;

        /// @brief gets the name of the assigned state
        /// @return the name of the assigned state
        std::string getName() const override;

        /// @brief creates a clone of the assigned state
        /// @return a pointer to the cloned assigned state
        TaskState* clone() const override { return new AssignedState(*this); }
};

#endif