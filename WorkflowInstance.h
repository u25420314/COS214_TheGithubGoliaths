#ifndef TIVIDY_WORKFLOWINSTANCE_H
#define TIVIDY_WORKFLOWINSTANCE_H

#include <string>
#include "WorkflowData.h"

class WorkNode;

/**
 * @class WorkflowInstance
 * @brief One running copy of a workflow, created from a WorkflowDefinition.
 *
 * An instance holds its own deep-cloned tree of WorkNodes (Prototype pattern), so
 * changing one instance never affects another. It also stores the instance's id,
 * its status and the WorkflowData supplied when it was started.
 *
 * The instance owns its tree: the destructor deletes the root. It cannot be copied.
 *
 * @see WorkflowDefinition
 * @see WorkflowData
 * @see WorkNode
 */
class WorkflowInstance {
public:

    /**
     * @brief Creates an instance that owns the given tree, with status "Running".
     * @param id   Unique id of this instance.
     * @param root Root of the instance's cloned tree. Ownership is transferred to
     *             the instance, so the caller must not delete it.
     */
    WorkflowInstance(const std::string& id, WorkNode* root);

    /**
     * @brief Destroys the instance and deletes its tree.
     */
    ~WorkflowInstance();

    /** @brief Copying is disabled, because the instance uniquely owns its tree. */
    WorkflowInstance(const WorkflowInstance&) = delete;

    /** @brief Copy assignment is disabled, because the instance uniquely owns its tree. */
    WorkflowInstance& operator=(const WorkflowInstance&) = delete;

    /**
     * @brief Gets the instance's unique id.
     * @return The id given at construction.
     */
    const std::string& getId() const;

    /**
     * @brief Gets the instance's current status (for example "Running").
     * @return The current status string.
     */
    const std::string& getStatus() const;

    /**
     * @brief Changes the instance's status.
     * @param s The new status.
     */
    void setStatus(const std::string& s);

    /**
     * @brief Gets the root of the instance's tree.
     * @return Pointer to the root node. It is owned by the instance and must not be deleted by the caller.
     */
    WorkNode* getRoot() const;

    /**
     * @brief Gets the data this instance was started with, for reading and modifying.
     * @return Reference to the instance's WorkflowData.
     */
    WorkflowData& getData();

    /**
     * @brief Gets the data this instance was started with, read-only.
     * @return Const reference to the instance's WorkflowData.
     */
    const WorkflowData& getData() const;

    /**
     * @brief Checks whether the whole workflow has finished.
     * @return true if the tree exists and its root reports it is complete, otherwise false.
     */
    bool isComplete() const;

private:
    std::string id;       ///< Unique id of this instance.
    std::string status;   ///< Current status; starts as "Running".
    WorkflowData data;    ///< Data supplied when the instance was started.
    WorkNode* root;       ///< Root of the owned, cloned tree.
};

#endif