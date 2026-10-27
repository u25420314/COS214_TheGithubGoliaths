#ifndef TIVIDY_DEPTHFIRSTITERATOR_H
#define TIVIDY_DEPTHFIRSTITERATOR_H

#include <stack>
#include "WorkNodeIterator.h"

class DepthFirstIterator : public WorkNodeIterator {
public:

    explicit DepthFirstIterator(WorkNode* root);

    void first() override;
    void next() override;
    bool isDone() const override;
    WorkNode* current() const override;

private:
    WorkNode* root;
    std::stack<WorkNode*> pending;
};

#endif
