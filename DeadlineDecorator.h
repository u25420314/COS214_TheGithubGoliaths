#ifndef DEADLINEDECORATOR_H
#define DEADLINEDECORATOR_H

#include "TaskDecorator.h"

/// @brief Class representing a deadline decorator for a task
class DeadlineDecorator : public TaskDecorator {
    protected:
        /// @brief performs the work of the deadline decorator
        void doWork() override;
    private:
        /// @brief number of days until the deadline
        int dueDays;
        /// @brief number of days elapsed
        int elapsed;
        /// @brief is the task late
        bool late;
    public:
        /// @brief contructor for the deadline decorator
        /// @param wrapped the work node to be wrapped by the deadline decorator
        /// @param dueDays the number of days until the deadline
        DeadlineDecorator(WorkNode* wrapped, int dueDays);

        /// @brief increase the deadline by a certain number of days
        /// @param days to increase the deadline by
        void advanceDays(int days); //adcance forwards

        /// @brief gets the number of days elapsed
        /// @return the number of days elapsed
        int daysElapsed() const;

        /// @brief gets the number of days until the deadline
        /// @return the number of days until the deadline
        int getDueDays() const;

        /// @brief checks if the task is overdue
        /// @return true if the task is overdue, false otherwise
        bool isOverdue() const;

        /// @brief checks if the task was completed late
        /// @return true if the task was completed late, false if not
        bool completedLate() const;

        /// @brief clones the deadline decorator
        /// @return the cloned deadline decorator
        WorkNode* clone() const override;
};
#endif