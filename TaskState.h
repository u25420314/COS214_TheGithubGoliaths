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

class Task;
class Participant;

/**
 * @class InvalidTransitionError
 * @brief Thrown when an action is not permitted in tasks current state
 * 
 * 
 * Lets callers (the TUI, commands, tests) distinguish a refused workflow
 * transition from other logics
 */
class InvalidTransitionError : public std::logic_error{
    public:

         /**
         * @brief Creates the error with a description of the refused action.
         * @param msg Human-readable explanation of why the transition failed.
         */
        explicit InvalidTransitionError(const std::string& msg) : std::logic_error(msg) {}
};

/**
 * @class TaskState
 * @brief Abstract base for every lifecycle state of a Task.
 *
 * GoF roles:
 * - State: Task is the Context and delegates each lifecycle action to its
 *   current TaskState. Concrete states override only the actions they allow
 * - Prototype: clone() lets a Task be deep-copied without knowing the concrete state type
 *
 * Every action defaults to refusing the transition by throwing InvalidTransitionError.
 */


class TaskState {
    protected:

        /**
         * @brief Rejects an action that is not allowed in this state
         * @param action Name of the attempted action 
         * @throws InvalidTransitionError always
         */
        void refuse(const std::string& action) const;
    public:

        /** @brief Virtual destructor so concrete states are deleted correctly */
        virtual ~TaskState() {}


        /**
         * @brief Begins work on the task
         * @param t The task whose state is changing
         * @throws InvalidTransitionError if starting is not allowed here
         */


        virtual void start(Task* t);
        /**
         * @brief Assigns the task to a participant
         * @param t The task whose state is changing
         * @param p The participant who will take responsibility for the task
         * @throws InvalidTransitionError if assignment is not allowed here.
         */

        
        virtual void assign(Task* t, Participant* p);

        virtual void complete(Task* t);
        virtual void reject(Task* t);
        virtual void escalate(Task* t);
        virtual void cancel(Task* t);

        virtual std::string getName() const = 0;
        virtual bool isTerminal() const {return false; }//returns true if no further transtions are possible

        virtual TaskState* clone() const = 0;
        virtual bool isComplete() const { return false; }
};

#endif