#pragma once
#include <cstddef>
#include <vector>
#include "WorkflowObserver.h"

/**
 * Subject half of the Observer pattern. WorkflowEngine inherits this, so
 * engine.attach / detach / notifyObservers match the class diagram.
 * Observers are NOT owned; the caller keeps them alive.
 */
class WorkflowSubject {
public:
    virtual ~WorkflowSubject() = default;
    void attach(WorkflowObserver* o);   ///< Ignores null and duplicates.
    void detach(WorkflowObserver* o);   ///< Safe to call during notifyObservers.
    void notifyObservers(const WorkflowEvent& e);
    std::size_t observerCount() const { return observers_.size(); }

private:
    std::vector<WorkflowObserver*> observers_;
};