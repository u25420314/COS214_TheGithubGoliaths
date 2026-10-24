#include "WorkflowSubject.h"
#include <algorithm>

void WorkflowSubject::attach(WorkflowObserver* o) {
    if (!o) return;
    if (std::find(observers_.begin(), observers_.end(), o) == observers_.end())
        observers_.push_back(o);
}

void WorkflowSubject::detach(WorkflowObserver* o) {
    observers_.erase(std::remove(observers_.begin(), observers_.end(), o), observers_.end());
}

void WorkflowSubject::notifyObservers(const WorkflowEvent& e) {
    // Iterate over a snapshot so observers may attach/detach while being notified.
    std::vector<WorkflowObserver*> snapshot = observers_;
    for (WorkflowObserver* o : snapshot) {
        // Skip anyone detached earlier in this same loop.
        if (std::find(observers_.begin(), observers_.end(), o) != observers_.end())
            o->update(e);
    }
}