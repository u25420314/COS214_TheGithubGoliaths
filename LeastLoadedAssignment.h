#ifndef LEASTLOADEDASSIGNMENT_H
#define LEASTLOADEDASSIGNMENT_H

#include "AssignmentStrategy.h"

class LeastLoadedAssignment : public AssignmentStrategy{
    public:
        virtual Participant* selectParticipant(consts std::vector<Participant*>& participants, const std::string& requiredRole);
};

#endif