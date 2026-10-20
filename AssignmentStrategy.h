#ifndef ASSIGNMENTSTRATEGY_H
#define ASSIGNMENTSTRATEGY_H

#include <vector>
#include <string>
#include "Participant.h"


class AssignmentStrategy{
    public:
        virtual Participant* selectParticipant(const std::vector<Participant*>& participants, const std::string& requiredRole) = 0;
        virtual ~AssignmentStrategy(){}
            
        
};

#endif