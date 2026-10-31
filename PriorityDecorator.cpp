#include "PriorityDecorator.h"

PriorityDecorator::PriorityDecorator(WorkNode* wrapped, int level) : TaskDecorator(wrapped), level(level) {}

int PriorityDecorator::getLevel() const {
    return level;
}

void PriorityDecorator::setLevel(int level) {
    this->level = level;
}

void PriorityDecorator::collectReady(std::vector<Task*>& out) {
    std::vector<Task*> mine;
    TaskDecorator::collectReady(mine);
    out.insert(out.begin(), mine.begin(), mine.end());
}

WorkNode* PriorityDecorator::clone() const {
    return new PriorityDecorator(wrapped ? wrapped->clone() : nullptr, level);
}