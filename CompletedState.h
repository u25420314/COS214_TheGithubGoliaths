#ifndef COMPLETEDSTATE_H
#define COMPLETEDSTATE_H

#include "TaskState.h"

/// @brief Class representing the completed state of a task
class CompletedState : public TaskState {
    public:
        /// @brief returns the name of the completed state
        /// @return the name of the completed state
        std::string getName() const override;

        /// @brief clones the completed state
        /// @return the completed state object
        TaskState* clone() const override { return new CompletedState(*this); }

        /// @brief checks if the completed state is terminal
        /// @return true if the completed state is terminal, false if not
        bool isTerminal() const override { return true; }

        /// @brief checks if the completed state is complete
        /// @return true if the completed state is complete, false if not
        bool isComplete() const override { return true; }
};

#endif