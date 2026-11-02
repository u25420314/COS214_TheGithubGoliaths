#ifndef TIVIDY_STAGE_H
#define TIVIDY_STAGE_H

#include <string>
#include <vector>
#include "WorkNode.h"

/// @brief Class representing a stage in a workflow, which can contain multiple child work nodes
class Stage : public WorkNode {
public:
    /// @brief constructor for the Stage class
    /// @param id the id for the stage
    /// @param name the name for the stage
    /// @param parallel if the stage is parallel or not
    Stage(const std::string& id, const std::string& name, bool parallel = false);

    /// @brief destructor for the Stage class
    ~Stage() override;

    /// @brief adds a child work node to the stage
    /// @param child the work node to be added as a child
    void add(WorkNode* child) override;

    /// @brief removes a child work node from the stage
    /// @param child the work node to be removed from the stage
    void remove(WorkNode* child) override;

    /// @brief gets the child work nodes of the stage
    /// @return a vector of child work nodes
    std::vector<WorkNode*> getChildren() const override;

    /// @brief sets the mediator for the stage
    void setMediator(WorkflowMediator* m) override;

    /// @brief returns whether the stage is parallel or not
    /// @return the parallel status of the stage
    bool isParallel() const;

    /// @brief gets the ready child work nodes of the stage
    /// @return a vector of ready child work nodes
    std::vector<WorkNode*> getReadyChildren() const;

    /// @brief clones the stage and its child work nodes
    /// @return the cloned stage object
    WorkNode* clone() const override;

    /// @brief creates an iterator for traversing the child work nodes of the stage
    /// @return the iterator for traversing the child work nodes of the stage
    WorkNodeIterator* createIterator() override;

    /// @brief determines if the stage is completed or not
    /// @return the completion status of the stage
    bool isComplete() const override;

    /// @brief collects the ready tasks from the stage and its child work nodes
    /// @param out the vector to store the ready tasks
    void collectReady(std::vector<Task*>& out) override;
protected:
    /// @brief determines if the stage can start based on its child work nodes
    /// @return the start status of the stage
    bool canStart() const override;

    /// @brief performs the work of the stage and its child work nodes
    void doWork() override;
private:
    /// @brief vector of child work nodes contained in the stage
    std::vector<WorkNode*> children;
    /// @brief indicates whether the stage is parallel or not
    bool parallel;
};

#endif
