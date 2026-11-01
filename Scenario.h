#ifndef SCENARIO_H
#define SCENARIO_H

#include <vector>

#include "Participant.h"

class WorkflowDefinition;

WorkflowDefinition* buildGoliathUniversityWorkflow();
WorkflowDefinition* buildInsuranceClaimWorkflow();
std::vector<Participant> scenarioParticipants();

#endif
