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
     * @brief Creates instance that owns the given tree, with status "Running"
     * @param id   Unique id for this instance
     * @param root Root of instance's cloned tree, ownership transferred to
     *             instance, therefore caller must not delete it
     */
    WorkflowInstance(const std::string& id, WorkNode* root);

    /**
     * @brief Destroys instance and deletes its tree
     */
    ~WorkflowInstance();

    /** @brief Copying is disabled, because instance uniquely owns its tree */
    WorkflowInstance(const WorkflowInstance&) = delete;

    /** @brief Copy assignment is disabled, because instance uniquely owns its tree */
    WorkflowInstance& operator=(const WorkflowInstance&) = delete;

    /**
     * @brief Gets instance's unique id
     * @return id given at construction
     */
    const std::string& getId() const;

    /**
     * @brief Gets instance's current status
     * @return Current status string
     */
    const std::string& getStatus() const;

    /**
     * @brief Changes instance's status
     * @param s New status
     */
    void setStatus(const std::string& s);

    /**
     * @brief Gets root of instance's tree
     * @return Pointer to root node, owned by the instance and mustn't be deleted by caller
     */
    WorkNode* getRoot() const;

    /**
     * @brief Gets data the instance was started with, for reading and modifying
     * @return Reference to instance's WorkflowData
     */
    WorkflowData& getData();

    /**
     * @brief Gets data this instance was started with, read-only
     * @return Const reference to instance's WorkflowData
     */
    const WorkflowData& getData() const;

    /**
     * @brief Checks whether whole workflow has finished
     * @return true if tree exists and root reports it is complete, otherwise false
     */
    bool isComplete() const;

private:
    std::string id;       ///< Unique id of instance
    std::string status;   ///< Current status, starts as "Running"
    WorkflowData data;    ///< Data supplied when instance was started
    WorkNode* root;       ///< Root of owned cloned tree
};

#endif