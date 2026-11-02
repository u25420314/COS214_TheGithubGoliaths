#ifndef SCENARIO_H
#define SCENARIO_H

#include <vector>

#include "Participant.h"

/// @brief Forward declaration of the WorkflowDefinition class
class WorkflowDefinition;

/// @brief builds the Goliath University workflow definition
/// @return the goliath university workflow definition
WorkflowDefinition* buildGoliathUniversityWorkflow();

/// @brief builds the insurance claim workflow definition
/// @return the insurance claim workflow definition
WorkflowDefinition* buildInsuranceClaimWorkflow();

/// @brief creates a vector of participants for the scenario
/// @return the vector of participants for the scenario
std::vector<Participant> scenarioParticipants();

#endif
