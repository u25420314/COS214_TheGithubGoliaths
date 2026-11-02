#ifndef LEASTLOADEDASSIGNMENT_H
#define LEASTLOADEDASSIGNMENT_H

#include "AssignmentStrategy.h"
/**
 * @class LeastLoadedAssignment
 * @brief Assigns tasks to the least busy eligible participant.
 */
class LeastLoadedAssignment : public AssignmentStrategy{
    public:
        /**
     * @brief Selects the participant with the lowest workload
     *        who has the task's required role.
     * @param task Task requiring assignment.
     * @param pool Available participants.
     * @return Least-loaded eligible participant, or nullptr if none.
     */
        virtual Participant* choose(const Task& task, const std::vector<Participant*>& pool);
};

#endif