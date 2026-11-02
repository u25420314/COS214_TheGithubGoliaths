#include "DepthFirstIterator.h"
#include "WorkNode.h"

//Constructor stores the root and calls first
DepthFirstIterator::DepthFirstIterator(WorkNode* root) {
    this->root = root;
    first();
}

//empties the pending stack and pushes root
void DepthFirstIterator::first() {
    while (!pending.empty()) {
        pending.pop();
    }
    if (root != nullptr) {
        pending.push(root);
    }
}

//pops current node and pushes its children in reverse so the first child comes off next
void DepthFirstIterator::next() {
    if (pending.empty()) {
        return;
    }
    WorkNode* node = pending.top();
    pending.pop();

    std::vector<WorkNode*> kids = node->getChildren();
    for (size_t i = kids.size(); i > 0; i--) {
        pending.push(kids[i - 1]);
    }
}

//true when stack is empty
bool DepthFirstIterator::isDone() const {
    return pending.empty();
}

//returns the top of the stack or nullptr if empty
WorkNode* DepthFirstIterator::current() const {
    if (pending.empty()) {
        return nullptr;
    }
    return pending.top();
}
