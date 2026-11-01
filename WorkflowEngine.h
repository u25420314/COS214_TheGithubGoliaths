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
 *
 * As Mediator, engine is single point that every WorkNode reports to
 * through notify(). Tasks never talk to each other: a task says what happened to it,
 * and the engine decides what happens next (activating follow-on tasks, detecting
 * that a workflow is finished, and fixing up the knock-on effects of an undo).
 *
 * As the **Subject** of the Observer pattern, the engine publishes a WorkflowEvent to
 * every attached WorkflowObserver (for example AuditLogger, NotificationService and
 * ProgressMonitor) whenever something happens.
 *
 * The engine owns every WorkflowInstance it creates. It does not own its observers.
 *
 * @note WorkNode, Task and WorkflowInstance must declare `friend class WorkflowEngine;`
 *       so the engine can read the private fields shown in the class diagram.
 *
 * @see WorkflowMediator
 * @see WorkflowObserver
 * @see WorkflowEvent
 * @see WorkflowInstance
 */

class WorkflowEngine : public WorkflowMediator {
public:
    /**
     * @brief Creates an engine with a pool of participants to assign work to.
     * @param participants The people tasks can be assigned to. Defaults to none.
     */
    explicit WorkflowEngine(std::vector<Participant> participants = {});
    
    /**
     * @brief Destroys the engine and every instance it owns.
     */
    ~WorkflowEngine() override;

    /**
     * @brief Starts a new running copy of a workflow.
     *
     * Clones the definition into a new instance, stores the supplied data in it,
     * points every node in the instance's tree at this engine, publishes an
     * "instance_created" event, and then activates the first tasks that are ready.
     *
     * @param def  The workflow definition (template) to instantiate.
     * @param data The data to store in the new instance.
     * @return Pointer to the new instance. It is owned by the engine and must not be deleted by the caller.
     */

    WorkflowInstance* createInstance(const WorkflowDefinition& def, const WorkflowData& data);

    /**
     * @brief Adds an observer that will receive every published event.
     *
     * Null pointers and observers that are already attached are ignored.
     *
     * @param o The observer to attach. It is not owned by the engine, so it must outlive its attachment.
     */

    void attach(WorkflowObserver* o);

    /**
     * @brief Removes an observer so it no longer receives events.
     *
     * Safe to call from inside an observer's update(), including to detach itself
     * or another observer while an event is being delivered.
     *
     * @param o The observer to remove. Does nothing if it is not attached.
     */

    void detach(WorkflowObserver* o);

    /**
     * @brief Delivers an event to every attached observer.
     *
     * Works from a snapshot of the observer list, so observers that detach during
     * delivery do not break the loop, and an observer detached earlier in the same
     * delivery is not called.
     *
     * @param e The event to publish.
     */

    void notifyObservers(const WorkflowEvent& e);

    /**
     * @brief Delivers an event to every attached observer.
     *
     * Works from a snapshot of the observer list, so observers that detach during
     * delivery do not break the loop, and an observer detached earlier in the same
     * delivery is not called.
     *
     * @param e The event to publish.
     */
    void notify(WorkNode* sender, const std::string& event) override;

        /**
     * @brief Mediator entry point: a node reports that something happened to it.
     *
     * The engine finds which instance the sender belongs to, publishes the event to
     * the observers unchanged, and then reacts:
     * - "completed" or "rejected": activates any tasks that are now ready. After
     *   "completed" it also publishes "instance_completed" if the whole workflow is done.
     * - "undone": returns downstream tasks that are no longer ready to Available.
     *
     * Events from nodes that do not belong to any instance are ignored.
     *
     * @param sender The node reporting the event.
     * @param event  What happened, for example "completed", "rejected", "escalated" or "undone".
     */
    void activateNext(WorkflowInstance& i);

    /**
     * @brief Assigns every task in the instance that is Available and ready to start.
     *
     * A task is ready when its dependencies are complete. This is what moves a
     * workflow forward after each task finishes.
     *
     * @param i The instance to activate tasks in.
     */
    void assign(Task* t);

    /**
     * @brief Looks up a task by its id.
     * @param taskId The id of the task to find.
     * @return Pointer to the task, or nullptr if no task has that id. The task is owned by its instance.
     */
    Task* findTask(const std::string& taskId);

private:
    /**
     * @brief Looks up a task by its id.
     * @param taskId The id of the task to find.
     * @return Pointer to the task, or nullptr if no task has that id. The task is owned by its instance.
     */
    WorkflowInstance* findInstanceOf(WorkNode* node);

    /**
     * @brief Handles the knock-on effects of an undo.
     *
     * Any task that is Assigned but can no longer start (because the task it
     * depended on was undone) is returned to Available, and "task_unassigned" is published.
     *
     * @param i The instance to check.
     */
    void unlockDownstream(WorkflowInstance& i);

    /**
     * @brief Builds a WorkflowEvent and sends it to the observers.
     * @param type The event type, for example "task_assigned".
     * @param i    The instance the event is about.
     * @param node The node the event is about, or nullptr for instance-wide events.
     */
    void publish(const std::string& type, WorkflowInstance& i, WorkNode* node);

    std::vector<std::unique_ptr<WorkflowInstance>> instances;
    std::vector<Participant> participants;
    std::vector<WorkflowObserver*> observers;
};