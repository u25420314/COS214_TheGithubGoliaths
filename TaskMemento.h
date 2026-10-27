//Memento : Memento

#ifndef TASKMEMENTO_H
#define TASKMEMENTO_H

#include "WorklowData.h"

#include <string>

using namespace std;

/**
 * @brief Stores the internal state of a task for recovery later on
 */
class TaskMemento {
    private:
        string stateName; ///< The name of the tasksState
        string assigneeid; ///< The id of the assignee
        WorkflowData payload; ///< The data that the task needs
        string timestamp; ///< The time it was made
    public:
        /// @brief Creates the memento
        /// @param stateName the stateName passed in
        /// @param assigneeid the assigneeid passed in
        /// @param payload the payload passed in
        /// @param timestamp the timestamp passed in
        TaskMemento(string stateName, string assigneeid, WorkflowData payload, string timestamp);
};

#endif