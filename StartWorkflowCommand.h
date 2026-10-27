//Command : ConcreteCommand

#ifndef STARTWORKFLOWCOMMAND_H
#define STARTWORKFLOWCOMMAND_H

#include "WorkFlowDefinition.h"
#include "WorkflowData.h"
#include "Command.h"

#include <vector>
#include <string>

using namespace std;

class WorkflowDefinition

/**
 * @brief The command class that starts a new instance of a workflow definition
 */
class StartWorkflowCommand : public Command {
    private:
        WorkflowDefinition* definition; ///< The definition that is used for the instance
        WorkflowData data; ///< The data to be used in the instance
        string createdId; ///< The id of the instance

    public:
        /// @brief This creates the StartWorkFlowCommand
        /// @param e The engine that stores the instance
        /// @param d The definition that is being used
        /// @param data The data to be used in the instance
        StartWorkflowCommand(WorkflowEngine* e, WorkflowDefinition* d, WorkflowData data);

        /// @brief Starts the instance
        void execute();

        /// @brief undoes the action performed by execute
        void undo();

        /// @brief gets the created ID
        /// @return createdId
        string getCreatedId() const;
};

#endif