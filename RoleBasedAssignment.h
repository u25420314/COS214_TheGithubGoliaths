#ifndef ROLEBASEDASSIGNMENT_H
#define ROLEBASEDASSIGNMENT_H

#include "AssignmentStrategy.h"

/// @brief Class representing a role-based assignment strategy for assigning tasks to participants
class RoleBasedAssignment : public AssignmentStrategy{
    public:
        /// @brief chooses a participant from the pool based on the role-based assignment strategy
        /// @param task the task that needs to be assigned
        /// @param pool the pool of participants to choose from
        /// @return the participant chosen off the requirement
        virtual Participant* choose(const Task& task, const std::vector<Participant*>& pool);
};

#endif