#pragma once
#include <string>
#include "WorkflowObserver.h"

/**
 * @class ProgressMonitor
 * @brief Observer that tracks how many tasks have been completed
 * @author Simon Vogel
 * 
 * Attach to WorkflowEngine and listens to each WorkflowEvent
 * "completed" event adds one to count, "undone" event removes one
 *
 * @see WorkflowObserver
 * @see WorkflowEngine
 * @see WorkflowEvent
 */
class ProgressMonitor : public WorkflowObserver {
public:
    /**
     * @brief Receives event from engine, updates count
     *
     * "completed"-> increment count, "undone" -> decrement count,
     * never below zero. All other event types ignored
     *
     * @param e The event that was published
     */
    void update(const WorkflowEvent& e) override;

    /**
     * @brief Builds formatted progress summary
     * @return String, e.g. "2 task(s) completed"
     */
    std::string report() const;

    /**
     * @brief Gets the number of tasks currently counted as completed.
     * @return The completed count (never negative)
     */
    int getCompletedCount() const { return completedCount; }

private:
    /**
     * @brief Number of tasks currently counted as completed
     */
    int completedCount = 0;   
};