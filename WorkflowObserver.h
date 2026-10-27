#pragma once
#include "WorkflowEvent.h"

/** Observer interface: anything that wants to hear about workflow events. */
class WorkflowObserver {
public:
    virtual ~WorkflowObserver() = default;
    virtual void update(const WorkflowEvent& e) = 0;
};