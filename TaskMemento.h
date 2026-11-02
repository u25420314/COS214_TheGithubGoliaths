//Memento : Memento

#ifndef TASKMEMENTO_H
#define TASKMEMENTO_H

#include "WorkflowData.h"
#include "TaskState.h"
#include "Participant.h"

#include <string>

using namespace std;

/**
 * @brief Stores the internal state of a task for recovery later on
 */
class TaskMemento {
    private:
        TaskState* state; ///< The name of the tasksState
        Participant* assignee; ///< The id of the assignee
    public:
        /// @brief Creates the memento
        /// @param state the stateName passed in
        /// @param assignee the assigneeid passed in
        TaskMemento(TaskState* state, Participant* assignee);

        /// @brief Gets the State
        /// @return returns stateName
        TaskState* getState() const;

        /// @brief gets the participant
        /// @return returns assigneeId
        Participant* getAssignee() const;

        ~TaskMemento();
};

#endif