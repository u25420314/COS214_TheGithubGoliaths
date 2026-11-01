#ifndef RULEBASEDASSIGNMENT_H
#define RULEBASEDASSIGNMENT_H

#include "AssignmentStrategy.h"

/// @brief Class representing a rule-based assignment strategy for assigning tasks to participants
class RuleBasedAssignment : public AssignmentStrategy{
    private:
        /// @brief the field to be checked in the participant's roles
        std::string field;
        /// @brief the threshold value for the field to determine eligibility
        std::string threshold;

    public:
        /// @brief constructor for RuleBasedAssignment
        /// @param field the field to be checked in the participant's roles
        /// @param threshold the threshold value for the field to determine eligibility
        RuleBasedAssignment(const std::string& field, const std::string& threshold);

        /// @brief chooses a participant from the pool based on the rule-based assignment strategy
        /// @param task the task that needs to be assigned
        /// @param pool the pool of participants to choose from
        /// @return the participant chosen based on the rule defined by the field and threshold
        virtual Participant* choose(const Task& task, const std::vector<Participant*>& pool);
};

#endif