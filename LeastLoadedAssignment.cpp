#include "LeastLoadedAssignment.h"
#include "Task.h"

Participant* LeastLoadedAssignment::choose(const Task& task, const std::vector<Participant*>& pool){
    Participant* selected = NULL;
    const std::string& requiredRole = task.getRequiredRole();

    for(size_t i = 0; i < pool.size(); i++){
        Participant* participant = pool[i];

        if(participant == NULL){
            continue;
        }

        if(!participant->hasRole(requiredRole)){
            continue;
        }

        if(selected == NULL || participant->getWorkload() < selected->getWorkload()){
            selected = participant;
        }
    }
    return selected;
}