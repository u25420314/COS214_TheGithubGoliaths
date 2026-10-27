#ifndef LEASTLOADEDASSIGNMENT_H
#define LEASTLOADEDASSIGNMENT_H

#include "AssignmentStrategy.h"

class LeastLoadedAssignment : public AssignmentStrategy{
    public:
        virtual Participant* choose(const Task& task, const std::vector<Participant*>& pool);
};

#endif