#include "ProgressMonitor.h"

void ProgressMonitor::update(const WorkflowEvent& e) {
    if (e.getType() == "completed") ++completedCount;
    else if (e.getType() == "undone" && completedCount > 0) --completedCount;
}

std::string ProgressMonitor::report() const {
    return std::to_string(completedCount) + " task(s) completed";
}