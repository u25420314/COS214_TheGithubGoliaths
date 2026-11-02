#pragma once
#include <memory>
#include <string>
#include <vector>
#include "WorkflowMediator.h"
#include "WorkflowObserver.h"
#include "Participant.h"

class Task;
class WorkNode;
class WorkflowData;
class WorkflowDefinition;
class WorkflowInstance;

/**
 * @class WorkflowEngine
 * @brief Mediator and Subject of workflow system
 * @author Simon Vogel and Nathan Spavins 
 * 
 * As Mediator, engine is single point that every WorkNode reports to
 * through notify(). Tasks never talk to one other: each task only notifies,
 * and engine decides course of action (activating follow-on tasks, detecting
 * that workflow is finished, resolving an undo)
 *
 * As Subject of the Observer pattern, engine publishes WorkflowEvent to
 * each attached WorkflowObserver, the engine owns each WorkflowInstance it creates. 
 * It does not own its observers.
 *
 * @note WorkNode, Task and WorkflowInstance must declare friend class
 *       so the engine can read private fields 
 *
 * @see WorkflowMediator
 * @see WorkflowObserver
 * @see WorkflowEvent
 * @see WorkflowInstance
 */

class WorkflowEngine : public WorkflowMediator {
public:
    /**
     * @brief Creates engine with pool of participants to assign work to
     * @param participants Individuals tasks are assigned to, defaults to none
     */
    explicit WorkflowEngine(std::vector<Participant> participants = {});
    
    /**
     * @brief Destroys engine and each instance it owns
     */
    ~WorkflowEngine() override;

    /**
     * @brief Starts new running copy of workflow
     *
     * Clones definition into instance, stores supplied data therein,
     * points every node in instance's tree at engine, publishes an
     * "instance_created" event, then activates ready first tasks
     *
     * @param def  Workflow definition to instantiate
     * @param data Data to store in new instance
     * @return Pointer to new instance.
     */

    WorkflowInstance* createInstance(const WorkflowDefinition& def, const WorkflowData& data);

    /**
     * @brief Adds observer that will receive each published event
     *
     * Already attached null pointers and observers are ignored
     *
     * @param o Observer to attach, not owned by engine
     */

    void attach(WorkflowObserver* o);

    /**
     * @brief Removes observer so it no longer receives events
     *
     * Safe from inside an observer's update(), including to detach itself
     * or another observer while an event is being delivered
     *
     * @param o The observer to remove, does nothing if it isn't attached
     */

    void detach(WorkflowObserver* o);

    /**
     * @brief Delivers an event to each attached observer
     *
     * Works from a snapshot of observer list, so observers that detach during
     * delivery don't break the loop, whereas an observer detached earlier 
     * in this delivery is not called.
     *
     * @param e event to publish.
     */

    void notifyObservers(const WorkflowEvent& e);

     /**
     * @brief Delivers an event to each attached observer
     *
     * Works from a snapshot of observer list, so observers that detach during
     * delivery don't break the loop, whereas an observer detached earlier 
     * in this delivery is not called.
     *
     * @param e event to publish.
     */
    void notify(WorkNode* sender, const std::string& event) override;

    /**
     * @brief Mediator entry point, node reports event
     *
     * Engine finds which instance sender belongs to, publishes the event to
     * observers unchanged, then reacts:
     * - "completed"/ "rejected": activates any tasks that are ready. After
     *   "completed," also publishes "instance_completed" if whole workflow is done
     * - "undone": returns downstream tasks that aren't ready anymore to Available
     *
     * Events from nodes that don't belong to any instance are ignored
     *
     * @param sender Node reporting the event
     * @param event  Stated as "completed", "rejected", "escalated" or "undone".
     */
    void activateNext(WorkflowInstance& i);

    /**
     * @brief Assigns each task in instance that is Available and ready to start
     *
     * A task is ready when its dependencies are complete, this is what moves a
     * workflow forward after each task finishes.
     *
     * @param i Instance to activate tasks within
     */
    void assign(Task* t);

    /**
     * @brief Looks up task by its id
     * @param taskId Id of task to find
     * @return Pointer to task or nullptr otherwise. 
     */
    Task* findTask(const std::string& taskId);

private:
    /**
     * @brief Looks up an Instance by its workNode
     * @param node Address of node to search with
     * @return Pointer to the instance, or nullptr if no Instance has that node
     */
    WorkflowInstance* findInstanceOf(WorkNode* node);

    /**
     * @brief Handles undo
     *
     * Any task that is Assigned but can't start (if the task it
     * depended on was undone) anymore is returned to Available, and "task_unassigned" is published
     *
     * @param i Instance to check
     */
    void unlockDownstream(WorkflowInstance& i);

    /**
     * @brief Builds WorkflowEvent, sends it to observers
     * @param type Event type 
     * @param i    Instance event is about
     * @param node Node event is about, or nullptr for instance-wide
     */
    void publish(const std::string& type, WorkflowInstance& i, WorkNode* node);

    std::vector<std::unique_ptr<WorkflowInstance>> instances;
    std::vector<Participant> participants;
    std::vector<WorkflowObserver*> observers;
};