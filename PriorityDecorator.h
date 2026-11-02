#ifndef PRIORITYDECORATOR_H
#define PRIORITYDECORATOR_H

#include "TaskDecorator.h"

/// @brief Class representing a priority decorator for a task
class PriorityDecorator : public TaskDecorator {
    private:
        /// @brief the priority level of the task
        int level;
    public:
        /// @brief constructor for the priority decorator
        /// @param wrapped the work node to be wrapped by the priority decorator
        PriorityDecorator(WorkNode* wrapped, int level);

        /// @brief gets the priority level of the task
        /// @return the priority level of the task
        int getLevel() const;

        /// @brief sets the priority level of the task
        /// @param level the new priority level to be set
        void setLevel(int level);

        /// @brief collects the ready tasks based on priority
        /// @param out a vector to store the ready tasks
        void collectReady(std::vector<Task*>& out) override;

        /// @brief clones the priority decorator
        /// @return the cloned priority decorator
        WorkNode* clone() const override;
};
#endif