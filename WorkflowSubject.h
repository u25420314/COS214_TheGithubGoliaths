#pragma once
#include <cstddef>
#include <vector>
#include "WorkflowObserver.h"


class WorkflowSubject {
public:
    virtual ~WorkflowSubject() = default;
    void attach(WorkflowObserver* o);   ///< Ignores null and duplicates
    void detach(WorkflowObserver* o);   ///< Safe for notifyObservers
    void notifyObservers(const WorkflowEvent& e);
    std::size_t observerCount() const { return observers_.size(); }

private:
    std::vector<WorkflowObserver*> observers_;
};