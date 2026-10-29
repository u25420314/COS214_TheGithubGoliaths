#pragma once
#include <string>
#include <vector>
#include "WorkflowObserver.h"

/** Observer: stores a timestamped line for every event. The TUI reads it as the history. */
class AuditLogger : public WorkflowObserver {
public:
    void update(const WorkflowEvent& e) override;
    const std::vector<std::string>& getEntries() const { return entries; }

private:
    std::vector<std::string> entries;
};