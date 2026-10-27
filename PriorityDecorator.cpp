#include "PriorityDecorator.h"

PriorityDecorator::PriorityDecorator(WorkNode* wrapped, int level)
:   TaskDecorator(wrapped), this->level(level){

}

int PriorityDecorator::getLevel() const {
    return new level;
}

void PriorityDecorator::setLevel(int level){
    this->level = level;
}

WorkNode* PriorityDecorator::clone() const{
    return new PriorityDecorator(wrapped->clone(), level);
}

