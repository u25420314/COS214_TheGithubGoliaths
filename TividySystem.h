//Facade : Facade
//Command : Client

#ifndef TIVIDYSYSTEM_H
#define TIVIDYSYSTEM_H

#include "WorkFlowEngine.h"
#include "CommandHistory.h"
#include "WorkflowDefinition.h"
#include "Participant.h"

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
        WorkFlowEngine engine; ///< The Mediator subsystem
        CommandHistory history; ///< The Command subsystem
        vector<WorkflowDefinition*> definitions; ///< The registered workflow definitions

    public:
        /**
         * @brief The constructor sets up the System
         */
        TividySystem();

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

        /// @brief undo the last action recently done
        void undoLast();

        /// @brief get the progress of a certain instance 
        /// @param instanceId The id of the instance to get the progress of
        /// @return The progress returned from the engine
        string getProgress(string instanceId);

        /// @brief Destruct the object by deleting its definition vector
        ~TividySystem();
};

#endif