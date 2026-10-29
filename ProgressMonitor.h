#pragma once
#include <string>
#include "WorkflowObserver.h"

/** Observer: counts completed tasks ("completed" adds one, "undone" removes one). */
class ProgressMonitor : public WorkflowObserver {
public:
    void update(const WorkflowEvent& e) override;
    std::string report() const;
    int getCompletedCount() const { return completedCount; }

private:
    int completedCount = 0;
};