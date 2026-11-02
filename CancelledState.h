#ifndef CANCELLEDSTATE_H
#define CANCELLEDSTATE_H

#include "TaskState.h"

/// @brief Class representing the cancelled state of a task
class CancelledState : public TaskState {
    public:
        /// @brief checks if the cancelled state is terminal
        /// @return true if the cancelled state is terminal, false otherwise
        bool isTerminal() const override {return true; } //every action is refusedd since task was canclled

        /// @brief gets the name of the cancelled state
        /// @return returns the name of the cancelled state
        std::string getName() const override;

        /// @brief creates a clone of the cancelled state
        /// @return a pointer to the cloned cancelled state
        TaskState* clone() const override { return new CancelledState(*this); }
};

#endif