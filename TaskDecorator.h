#ifndef TASKDECORATOR_H
#define TASKDECORATOR_H

#include "WorkNode.h"
#include "WorkflowMediator.h"

/// @brief The abstract decorator for a WorkNode
class TaskDecorator : public WorkNode {
    protected:
        /// @brief does the additional work of the decorator
        void doWork() override;
        /// @brief the WorkNode that the decorator owns
        WorkNode* wrapped; //owned variable
        /// @brief finishes the decorated additional responsibilities
        void finish() override;
    public:
        /// @brief The constructor for the TaskDecorator
        /// @param wrapped the workNode the decorator wraps
        explicit TaskDecorator(WorkNode* wrapped);
        /// @brief The destructor for the decorator
        virtual ~TaskDecorator();

        /// @brief gets the children of the decorator
        /// @return the children of the decorator
        std::vector<WorkNode*> getChildren() const override;

        /// @brief creates an iterator to traverse the Decorator
        /// @return the iterator to traverse the decoratoe
        WorkNodeIterator* createIterator() override;

        /// @brief checks if the decorators responsibilities are completed
        /// @return if its true or false
        bool isComplete() const override;

        /// @brief sets the mediator for the decorator
        /// @param m the mediator being assigned
        void setMediator(WorkflowMediator* m) override;

        /// @brief collects all the ready tasks inside the decorator 
        /// @param out the ready tasks
        void collectReady(std::vector<Task*>& out) override;

        /// @brief checks if it can start
        /// @return if it can start or not
        bool canStart() const override;

        /// @brief clones the decorator
        /// @return the cloned decorator
        WorkNode* clone() const override = 0;
};

#endif