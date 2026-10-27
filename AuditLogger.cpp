#include "AuditLogger.h"

void AuditLogger::update(const WorkflowEvent& e) {
    std::string line = "[" + e.getTime() + "] " + e.getType() +
                       " instance=" + e.getInstanceId();
    if (!e.getNodeId().empty()) line += " node=" + e.getNodeId();
    entries_.push_back(line);
}