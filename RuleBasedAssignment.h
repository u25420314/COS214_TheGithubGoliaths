#ifndef RULEBASEDASSIGNMENT_H
#define RULEBASEDASSIGNMENT_H

#include "AssignmentStrategy.h"

/**
 * @class RuleBasedAssignment
 * @brief Selects participants according to the rules asked
 * 
 * This class is a Concrete startegy which does 1 o 2 things depending on the input
 * it uses a rule field and threshhold to detrmine which participants are elegible for the assignent
 * 
 * Supported rules include checking for an additonal role and the other rule is if
 * to set a max workload
 */

class RuleBasedAssignment : public AssignmentStrategy{
    private:
        /// @brief the field to be checked in the participant's roles
        std::string field;
        /// @brief the threshold value for the field to determine eligibility
        std::string threshold;

    public:
        /**
         * @breif constructs a rule-based-assignment startegy
         * 
         * @param field the rule which will be applied
         * @param threshold the contents of the rule which will be applied
         */
        RuleBasedAssignment(const std::string& field, const std::string& threshold);
        /**
         * @brief Selects the 1st participant that satisfiesthe assignemnt rule
         * 
         * iterates through the pool of participants and checks whether the participant 
         * has the role required by the task
         * 
         * depending on the rule the next oart changes
         * 
         * if its "role" the participants must also have the aditional required role then they will be chosesen as the particpant
         * 
         * if its "maxWorkload" the participants current wokrload must be lowee then the amount specified in the threshold
         * 
         * @param task the task that needs to be assigned
         * @param participants a list of available participant pointers
         * 
         * @return Participant* Pointer to the first participant who 
         * satisfies the required role and the additonal rule it returns nullptr if no one suitable is found
         */
        virtual Participant* choose(const Task& task, const std::vector<Participant*>& pool);
};

#endif