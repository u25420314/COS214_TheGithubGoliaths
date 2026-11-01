#ifndef LEASTLOADEDASSIGNMENT_H
#define LEASTLOADEDASSIGNMENT_H

#include "AssignmentStrategy.h"

/// @brief Class representing the least loaded assignment strategy for assigning tasks to participants
class LeastLoadedAssignment : public AssignmentStrategy{
    public:
        /// @brief chooses a participant from the pool based on the least loaded assignment strategy
        /// @param task the task that needs to be assigned
        /// @param pool the pool of participants to choose from
        /// @return the chosen participant with the least workload
        virtual Participant* choose(const Task& task, const std::vector<Participant*>& pool);
};

#endif