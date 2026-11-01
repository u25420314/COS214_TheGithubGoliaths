#pragma once
#include <string>
#include "WorkflowObserver.h"

/// @brief class representing a progress monitor that observes workflow events
class ProgressMonitor : public WorkflowObserver {
public:
    /// @brief updates the progress monitor with a new workflow event
    /// @param e the event to be monitored
    void update(const WorkflowEvent& e) override;

    /// @brief reports the progress of completed tasks
    /// @return the report string containing the number of completed tasks
    std::string report() const;

    /// @brief gets the count of completed tasks
    /// @return the number of completed tasks
    int getCompletedCount() const { return completedCount; }
private:
    /// @brief the count of completed tasks
    int completedCount = 0;
};