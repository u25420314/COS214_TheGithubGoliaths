#ifndef TASKSTATE_H
#define TASKSTATE_H

/**
 * @file TaskState.h
 * @brief Abstract state in the state pattern for a Task's lifecycle, plus the exception
 * 
 * 
 */
#include <stdexcept>
#include <string>

/// @brief forward declaration of Task
class Task;
/// @brief forward declaration of Participant
class Participant;

class InvalidTransitionError : public std::logic_error{
    public:
        explicit InvalidTransitionError(const std::string& msg) : std::logic_error(msg) {}
};


class TaskState {
    protected:
        void refuse(const std::string& action) const;
    public:
        virtual ~TaskState() {}


        /**
         * @brief Begins work on the task
         * @param t The task whose state is changing
         * @throws InvalidTransitionError if starting is not allowed here
         */


        /// @brief starts the task State
        /// @param t the task to start
        virtual void start(Task* t);
        virtual void assign(Task* t, Participant* p);
        virtual void complete(Task* t);

        /// @brief reject a task
        /// @param t the task to be rejected
        virtual void reject(Task* t);

        /// @brief escalate a task
        /// @param t the task to be escalated
        virtual void escalate(Task* t);

        /// @brief cancel a task
        /// @param t the task to be canceled
        virtual void cancel(Task* t);

        /// @brief get the name of the task
        /// @return the name of the task
        virtual std::string getName() const = 0;

        /// @brief checks if the task is terminal
        /// @return if the task is terminal or not
        virtual bool isTerminal() const {return false; }//returns true if no further transtions are possible

        /// @brief clones the TaskState
        /// @return returns the cloned TaskState
        virtual TaskState* clone() const = 0;

        /// @brief checks if the task is complete or not
        /// @return if the task is complete or not
        virtual bool isComplete() const { return false; }
};

#endif