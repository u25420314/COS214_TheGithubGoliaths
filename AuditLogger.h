#pragma once
#include <string>
#include <vector>
#include "WorkflowObserver.h"

/** Observer that keeps a timestamped text history of every event (read by the TUI). */
class AuditLogger : public WorkflowObserver {
public:
    void update(const WorkflowEvent& e) override;
    const std::vector<std::string>& getEntries() const { return entries_; }

private:
    std::vector<std::string> entries_;
};