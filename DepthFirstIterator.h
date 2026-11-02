#ifndef TIVIDY_DEPTHFIRSTITERATOR_H
#define TIVIDY_DEPTHFIRSTITERATOR_H

#include <stack>
#include "WorkNodeIterator.h"

/// @brief Class representing a depth-first iterator for traversing work nodes
class DepthFirstIterator : public WorkNodeIterator {
public:
    /// @brief constructor for the depth-first iterator
    /// @param root the root work node from which the traversal begins
    explicit DepthFirstIterator(WorkNode* root);

    /// @brief move the iterator back to the start
    void first() override;

    /// @brief searches for the next work node in the depth-first traversal
    void next() override;

    /// @brief checks if the traversal is complete
    bool isDone() const override;

    /// @brief gets the current work node in the traversal
    /// @return the current work node in the traversal
    WorkNode* current() const override;
private:
    /// @brief the root work node where the traversal begins
    WorkNode* root;
    /// @brief stack to keep track of the pending work nodes during traversal
    std::stack<WorkNode*> pending;
};

#endif
