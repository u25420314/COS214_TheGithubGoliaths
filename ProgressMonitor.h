#pragma once
#include <string>
#include "WorkflowObserver.h"

/** Observer that counts completed tasks ("task_completed" up, "task_undone" down). */
class ProgressMonitor : public WorkflowObserver {
public:
    explicit ProgressMonitor(int totalTasks = 0) : total_(totalTasks) {}
    void update(const WorkflowEvent& e) override;
    void setTotal(int total) { total_ = total; }
    int getCompleted() const { return completedCount_; }
    int getTotal() const { return total_; }
    int percentage() const;                 ///< 0 if total is 0
    std::string report() const;             ///< e.g. "Progress: 2/5 tasks complete (40%)"

private:
    int completedCount_ = 0;
    int total_;
};