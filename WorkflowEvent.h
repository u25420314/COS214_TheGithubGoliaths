#pragma once
#include <string>
/**
 * @class WorkflowEvent
 * @author Simon Vogel
 * @brief
 * An event in a workflow, published by engine to observers
 * 
 * This class is meant to represent an event within the workflow, such as assigning
 *  a task or performing an action. It is used by WorkflowObserver to update WorkflowSubject
 * as well as to update the WorkflowEngine.   
 *
 * @see WorkflowInstance
 * @see AuditLogger
 * @see WorkflowObserver
 * @see WorkflowEngine
 * 
 */

class WorkflowEvent {
public:
    /**
     * @brief Constructor for a WorkflowEvent object
     * @param type The type of the WorkflowEvent
     * @param instanceId The id for the WorkflowInstance attached to this object
     * @param nodeId The id for the WorkNode effected by this WorkflowEvent
     */
    WorkflowEvent(std::string type, std::string instanceId, std::string nodeId);
    
    /**
     * @brief Getter for type 
     * @return String Type
     */
    const std::string& getType() const { return type; }
    
    /**
     * @brief Getter for the instanceId
     * @return String instanceId
     */
    const std::string& getInstanceId() const { return instanceId; }
    
    /**
     * @brief Getter for the NodeId
     * @return String NodeId
     */
    const std::string& getNodeId() const { return nodeId; }
    
    /**
     * @brief Getter for the time
     * @return String time
     */
    const std::string& getTime() const { return time; }

private:
    /**
     * The type of this WorkflowEvent, determined by WorkflowEngine
     */
    std::string type;

    /**
     * @brief The id for the WorkflowInstance this WorkflowEvent represents
     */
    std::string instanceId;

    /**
     * @brief The id for the WorkNode attached to this WorkflowEvent
     */
    std::string nodeId;

    /**
     * @brief The time, set automatically when WorkflowEvent is created
     */
    std::string time;   
};