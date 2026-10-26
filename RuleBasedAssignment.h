#ifndef RULEBASEDASSIGNMENT_H
#define RULEBASEDASSIGNMENT_H

#include "AssignmentStrategy.h"

class RuleBasedAssignment : public AssignmentStrategy{
    private:
        std::string field;
        std::string threshold;

    public:
        RuleBasedAssignment(const std::string& field, const std::string& threshold);
        virtual Participant* choose(const Task& task, const std::vector<Participant*>& pool);
};

#endif