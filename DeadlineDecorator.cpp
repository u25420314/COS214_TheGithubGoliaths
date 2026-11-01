#include "DeadlineDecorator.h"
#include <stdexcept>

DeadlineDecorator::DeadlineDecorator(WorkNode* wrapped, int dueDays)
: TaskDecorator(wrapped), dueDays(dueDays), elapsed(0), late(false){
    if (dueDays < 0)
    {
        throw std::invalid_argument("dueDays must be >= 0");
    }
    
}

void DeadlineDecorator::advanceDays(int days){
    if(days < 0){
        throw std::invalid_argument("cannot move time backwards");
        
    }

    elapsed += days;
}

int DeadlineDecorator::daysElapsed() const {
    return elapsed;
}

int DeadlineDecorator::getDueDays() const {
    return dueDays;
}

bool DeadlineDecorator::isOverdue() const {
    return elapsed > dueDays;
}

bool DeadlineDecorator::completedLate() const {
    return late;
}

void DeadlineDecorator::doWork() {
    late = isOverdue();
    TaskDecorator::doWork();
}

WorkNode* DeadlineDecorator::clone() const {
    return new DeadlineDecorator(wrapped ? wrapped->clone(): nullptr, dueDays);
}