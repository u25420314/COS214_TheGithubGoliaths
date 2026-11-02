/**
 * @file WorkNode.h
 * @brief Defines the WorkNode abstract base class for the Composite pattern.
 * 
 * This file is part of the Workflow Model subsystem (Composite pattern). 
 * WorkNode serves as the Component interface in the Composite hierarchy. 
 * It defines the common operations for both individual Tasks (Leaf) and 
 * Stages (Composite).
 */

#ifndef TIVIDY_WORKNODE_H
#define TIVIDY_WORKNODE_H

#include <string>
#include <vector>

class WorkflowMediator;
class WorkNodeIterator;
class Task;

/**
 * @class WorkNode
 * @brief abstract Component class in Composite
 * 
 * WorkNode defines interface for all workflow elements, Concrete subclasses 
 * implement specific behaviours for leaf nodes and composite nodes respectively. 
 * It also holds the reference to Mediator to let nodes communicate with WorkflowEngine
 */
class WorkNode {
public:

    /**
     * @brief Constructs WorkNode with unique identifier and display name
     * @param id Unique identifier for this node 
     * @param name name of the node
     */
    WorkNode(const std::string& id, const std::string& name);

    /**
     * @brief Virtual destructor, ensures proper cleanup of derived classes
     */
    virtual ~WorkNode();

    /// @brief Copy constructor is deleted, Nodes are unique and shouldn't be copied directly (use clone())
    WorkNode(const WorkNode&) = delete;

    /// @brief Copy assignment is deleted, Nodes are unique and shouldn't be copied directly
    WorkNode& operator=(const WorkNode&) = delete;

    /**
     * @brief Executes workflow logic for this node and its children
     * 
     * template method that delegates to doWork() for specific
     * implementation and finish() for post-processing
     */
    void run();

    /**
     * @brief Adds achild node to this node (Composite pattern operation).
     * @param child Pointer to WorkNode to add Ownership is transferred.
     * @note For leaf nodes (Task), this typically throws an exception or does nothing.
     */
    virtual void add(WorkNode* child);

    /**
     * @brief Removes a child node from this node (Composite pattern operation).
     * @param child Pointer to the WorkNode to remove.
     * @note For leaf nodes (Task), this typically throws an exception or does nothing.
     */
    virtual void remove(WorkNode* child);

    /**
     * @brief Retrieves the immediate children of this node.
     * @return A vector of pointers to the child WorkNodes.
     * @note For leaf nodes, this returns an empty vector.
     */
    virtual std::vector<WorkNode*> getChildren() const;

    /**
     * @brief Creates a deep copy of this node (Prototype pattern).
     * @return A pointer to the newly cloned WorkNode.
     */
    virtual WorkNode* clone() const = 0;

    /**
     * @brief Creates an iterator to traverse this node's subtree (Iterator pattern).
     * @return A pointer to a WorkNodeIterator. Ownership is passed to the caller.
     */
    virtual WorkNodeIterator* createIterator() = 0;

    /**
     * @brief Checks if this node has finished its execution.
     * @return True if complete, false otherwise.
     */
    virtual bool isComplete() const = 0;

    /**
     * @brief Gets the unique identifier of this node.
     * @return A const reference to the node's ID string.
     */
    const std::string& getId() const;

    /**
     * @brief Gets the display name of this node.
     * @return A const reference to the node's name string.
     */
    const std::string& getName() const;

    /**
     * @brief Sets the Mediator for this node.
     * @param m Pointer to the WorkflowMediator (typically the WorkflowEngine).
     */
    virtual void setMediator(WorkflowMediator* m);

    /**
     * @brief Gets the Mediator associated with this node.
     * @return A pointer to the WorkflowMediator.
     */
    WorkflowMediator* getMediator() const;

    /**
     * @brief Checks if this node is currently in a state that allows it to start.
     * @return True if the node can start, false otherwise.
     */
    virtual bool canStart() const;

    /**
     * @brief Casts this WorkNode to a Task pointer, if possible.
     * @return A Task pointer if this is a Task, otherwise nullptr.
     */
    virtual Task* asTask() { return nullptr; }

    /**
     * @brief Collects all child tasks that are ready to be executed.
     * @param out A reference to a vector where ready Task pointers will be appended.
     */
    virtual void collectReady(std::vector<Task*>& out) = 0;

protected:
    /**
     * @brief The core execution logic specific to the concrete node type.
     * 
     * This method is called by run(). Derived classes (like Stage or Task)
     * must implement this to define their specific behaviour.
     */
    virtual void doWork() = 0;

    /**
     * @brief Post-execution cleanup or state finalisation.
     * 
     * Called by run() after doWork() completes.
     */
    virtual void finish();

    std::string id; ///< Unique identifier for the node.
    std::string name; ///< Human-readable name.
    WorkflowMediator* mediator; ///< Pointer to the mediator for engine communication.
};

#endif