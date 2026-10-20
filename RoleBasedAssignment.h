#ifndef ROLEBASEDASSIGNMENT_H
#define ROLEBASEDASSIGNMENT_H

#include "AssignmentStrategy.h"

class RoleBasedAssignment : public AssignmentStrategy{
    public:
        virtual Participant* selectParticipant(consts std::vector<Participant*>& participants, const std::string& requiredRole);
};

#endif