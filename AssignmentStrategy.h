#ifndef ASSIGNMENTSTRATEGY_H
#define ASSIGNMENTSTRATEGY_H

#include <vector>
#include <string>
#include "Participant.h"

class Task;

/// @brief Abstract base class for assignment strategies
class AssignmentStrategy{
    public:
        /// @brief chooses a participant from the pool based on the assignment strategy
        /// @param task the task that needs to be assigned
        /// @param pool the pool of participants to choose from
        /// @return the chosen participant
        virtual Participant* choose(const Task& task, const std::vector<Participant*>& pool) = 0;
        
        /// @brief virtual destructor for AssignmentStrategy
        virtual ~AssignmentStrategy(){}
};

#endif