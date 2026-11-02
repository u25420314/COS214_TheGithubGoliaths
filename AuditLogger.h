#pragma once
#include <string>
#include <vector>
#include "WorkflowObserver.h"

/// @brief class representing an audit logger that observes workflow events
class AuditLogger : public WorkflowObserver {
public:
    /// @brief updates the audit logger with a new workflow event
    /// @param e the event to be logged
    void update(const WorkflowEvent& e) override;

    /// @brief gets the entries logged by the audit logger
    /// @return returns the entries logged by the audit logger
    const std::vector<std::string>& getEntries() const { return entries; }

private:
    /// @brief vector of entries logged by the audit logger
    std::vector<std::string> entries;
};