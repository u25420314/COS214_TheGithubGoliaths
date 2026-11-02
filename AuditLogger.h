#pragma once
#include <string>
#include <vector>
#include "WorkflowObserver.h"

/** 
 * @class AuditLogger 
 * @author Simon Vogel
 * @brief AuditLogger: concrete instance of WorkflowObserver, stores a timestamped line
 * for each event, read by the GUI as the history of the WorkflowEvents
 * 
 * @see WorkflowObserver
 */

class AuditLogger : public WorkflowObserver {
public:
    /**
     * @brief Overrides update to generate a vector of entries representing the timestamped WorkflowEvent history
     * @param e the WorkflowEvent being added to the history 
     */
    void update(const WorkflowEvent& e) override;
    
    /**
     * @brief Returns the entries of all the timestamped WorkflowEvents as formatted strings
     * @return Vector of all entries, formatted
     */
    const std::vector<std::string>& getEntries() const { return entries; }

private:
    /**
    * @brief vector storing the strings representing the timestamped history of the Workflowevents 
    */
    std::vector<std::string> entries;
};