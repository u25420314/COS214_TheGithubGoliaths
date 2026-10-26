#ifndef ROLEBASEDASSIGNMENT_H
#define ROLEBASEDASSIGNMENT_H



#include "AssignmentStrategy.h"

class RoleBasedAssignment : public AssignmentStrategy{
    public:
        virtual Participant* choose(const Task& task, const std::vector<Participant*>& pool);
};

#endif