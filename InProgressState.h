#ifndef INPROGRESSSTATE_H
#define INPROGRESSSTATE_H

#include "TaskState.h"

/// @brief Class representing the in-progress state of a task
class InProgressState : public TaskState {
    public:
        /// @brief completes the task in the in-progress state
        /// @param t the task to be completed
        void complete(Task* t) override;

        /// @brief rejects the task in the in-progress state
        /// @param t the task to be rejected
        void reject(Task* t) override;

        /// @brief escalates the task in the in-progress state
        /// @param t the task to be escalated
        void escalate(Task* t) override;

        /// @brief cancels the task in the in-progress state
        /// @param t the task to be canceled
        void cancel(Task* t) override;

        /// @brief gets the name of the in-progress state
        /// @return the name of the in-progress state
        std::string getName() const override;

        /// @brief makes a clone of the in-progress state
        /// @return the cloned in-progress state object
        TaskState* clone() const override { return new InProgressState(*this); }
};

#endif