#ifndef ROLEBASEDASSIGNMENT_H
#define ROLEBASEDASSIGNMENT_H

#include "AssignmentStrategy.h"
/**
 * @class LeastLoadedAssignment
 * @brief Assigns a task to to thr participant with the lowest workload
 * 
 * this class is a concrete Strategy, it selects a particapant who satisfies the required role
 * and currenlty has the smallest workload
 */
class RoleBasedAssignment : public AssignmentStrategy{
    public:
        /**
         * @brief selects participant with the smallest workload and has the required role
         * 
         * Iterates through the available participants and consider participants 
         * only with the required role and the least currently assigned participants
         * 
         * @param task the task that needs to be assigned
         * @param participants a pool of th available participants
         * @return participant who mathced the criteria or nullptr if no eligible participants
         */
        virtual Participant* choose(const Task& task, const std::vector<Participant*>& pool);
};

#endif