#pragma once
#include "WorkflowEvent.h"

/**
 * @class WorkflowObserver
 * @author Simon Vogel
 * @brief Abstract <<Interface>> for anything that wants to be notified about workflow events
 * 
 * @see WorkflowEngine
 * @see AuditLogger
 * @see ProgressMonitor //Inheritor
 * @see NotificationService //Inheritor
 * @see WorkflowManager //Inheritor
 *
 */

class WorkflowObserver {
public:

    /**
     * @brief WorkflowObserver destructor
     */
    virtual ~WorkflowObserver() = default;
    
    /**
     * @brief Abstract function to be overriden, causes all Observers to be updated 
     * @param WorkflowEvent which WorkflowEvent is causing the update
     */
    virtual void update(const WorkflowEvent& e) = 0;
};