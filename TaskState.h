#ifndef TASKSTATE_H
#define TASKSTATE_H

#include <stdexcept>
#include <string>

/// @brief forward declaration of Task
class Task;
/// @brief forward declaration of Participant
class Participant;

/// @brief class for and invalid transaction error
class InvalidTransitionError : public std::logic_error{
    public:
        /// @brief constructor for an invalid transaction error
        /// @param msg is the message of why theres an error
        explicit InvalidTransitionError(const std::string& msg) : std::logic_error(msg) {}
};

/// @brief the class for the task state
class TaskState {
    protected:
        /// @brief refuses a task
        /// @param action the action the task does
        void refuse(const std::string& action) const;
    public:
        /// @brief the destructor of the TaskState
        virtual ~TaskState() {}

        /// @brief starts the task State
        /// @param t the task to start
        virtual void start(Task* t);

        /// @brief assigns a task with the participant
        /// @param t the task to be assigned
        /// @param p the participant to recieve the task
        virtual void assign(Task* t, Participant* p);

        /// @brief complete a task
        /// @param t the task to be completed
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