#ifndef RULEBASEDASSIGNMENT_H
#define RULEBASEDASSIGNMENT_H

#include "AssignmentStrategy.h"

class RuleBasedAssignment : public AssignmentStrategy{
    private:
        int preferredParticipantId;

    public:
        virtual Participant* selectParticipant(consts std::vector<Participant*>& participants, const std::string& requiredRole);
};

#endif