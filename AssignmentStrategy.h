#ifndef ASSIGNMENTSTRATEGY_H
#define ASSIGNMENTSTRATEGY_H

#include <vector>
#include <string>
#include "Participant.h"

class Task;


class AssignmentStrategy{
    public:
        virtual Participant* choose(const Task& task, const std::vector<Participant*>& pool) = 0;
        virtual ~AssignmentStrategy(){}
            
        
};

#endif