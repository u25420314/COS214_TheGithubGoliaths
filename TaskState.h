#ifndef TASKSTATE_H
#define TASKSTATE_H

#include <stdexcept>
#include <string>

class Task;
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

        virtual void start(Task* t);
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