//Facade : Facade
//Command : Client

#ifndef TIVIDYSYSTEM_H
#define TIVIDYSYSTEM_H

#include "WorkflowEngine.h"
#include "CommandHistory.h"
#include "WorkflowDefinition.h"
#include "WorkflowData.h"
#include "Participant.h"
#include "StartWorkflowCommand.h"
#include "AssignTaskCommand.h"
#include "CompleteTaskCommand.h"
#include "StartTaskCommand.h"
#include "RejectTaskCommand.h"
#include "Task.h"
#include "AuditLogger.h"

#include <vector>
#include <string>

using namespace std;


/**
 * @brief This brings everyone together into one class so its easy to use the workflow
 * 
 * The facade of the system.  With CommandHistory, WorkflowEngine, WorkflowDefinition
 */
class TividySystem {
    private:
        WorkflowEngine engine; ///< The Mediator subsystem
        AuditLogger auditLogger; ///< Records every event (Observer)
        CommandHistory history; ///< The Command subsystem
        vector<WorkflowDefinition*> definitions; ///< The registered workflow definitions

        

    public:
        /**
         * @brief The constructor sets up the System
         * @param particpants The participants involved in the system
         */
        TividySystem(vector<Participant> participants = {});

        /// @brief Adds a new Definition to the definitions vector
        /// @param d The definition to be added
        void registerDefinition(WorkflowDefinition* d);

        /// @brief start the a new instance of a definitions
        /// @param data The data to be implemented in the definition
        /// @param defName The name of the definition to implement
        /// @return The id of the instance
        string startWorkflow(string defName, WorkflowData data);

        /// @brief Give a task to a participant in the system
        /// @param taskId The id of the task to search for
        /// @param p The participant to assign the task to
        void assignTask(string taskId, Participant* p);

        /// @brief Marks an assigned task as started
        /// @param taskId
        void startTask(string taskId);

        /// @brief Marks an assigned task as completed
        /// @param taskId
        void completeTask(string taskId);

        /// @brief Marks an assigned task as rejected
        /// @param taskId
        void rejectTask(string taskId);

        /// @brief undo the last action recently done
        void undoLast();

        /// @brief Get every event recorded so far
        /// @return the history
        vector<string> getHistory() const;

        /// @brief Destruct the object by deleting its definition vector
        ~TividySystem();
};

#endif