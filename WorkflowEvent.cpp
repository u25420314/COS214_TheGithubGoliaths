#include "WorkflowEvent.h"
#include <ctime>
#include <iomanip>
#include <sstream>

WorkflowEvent::WorkflowEvent(std::string type, std::string instanceId, std::string nodeId) : type(std::move(type)), instanceId(std::move(instanceId)), nodeId(std::move(nodeId)) {}