#include "RoleBasedAssignment.h"
#include "Task.h"

Participant* RoleBasedAssignment::choose(const Task& task, const std::vector<Participant*>& pool){
    const std::string& requiredRole = task.getRequiredRole();
    
    for(size_t i = 0; i < pool.size(); i++){
        Participant* participant = pool[i];

        if(participant != NULL && participant->hasRole(requiredRole)){
            return participant;
        }
    }

    return NULL;
}