#ifndef ESCALATEDSTATE_H
#define ESCALATEDSTATE_H

#include "TaskState.h"

/// @brief Class representing the escalated state of a task
class EscalatedState : public TaskState {
    public:
        /// @brief assigns the task to a participant in the escalated state
        /// @param t the task to be assigned
        /// @param p the participant that is assigned the task
        void assign(Task* t, Participant* p) override;

        /// @brief gets the name of the escalated state
        /// @return the name of the escalated state
        std::string getName() const override;

        /// @brief creates a clone of the escalated state
        /// @return a pointer to the cloned escalated state
        TaskState* clone() const override { return new EscalatedState(*this); }
};

#endif