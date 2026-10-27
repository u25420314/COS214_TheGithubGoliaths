#include "ProgressMonitor.h"

void ProgressMonitor::update(const WorkflowEvent& e) {
    if (e.getType() == "task_completed") ++completedCount_;
    else if (e.getType() == "task_undone" && completedCount_ > 0) --completedCount_;
}

int ProgressMonitor::percentage() const {
    return total_ > 0 ? (completedCount_ * 100) / total_ : 0;
}

std::string ProgressMonitor::report() const {
    return "Progress: " + std::to_string(completedCount_) + "/" + std::to_string(total_) +
           " tasks complete (" + std::to_string(percentage()) + "%)";
}