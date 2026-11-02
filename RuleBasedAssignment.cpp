#include "RuleBasedAssignment.h"
#include "Task.h"

#include <sstream>

RuleBasedAssignment::RuleBasedAssignment(const std::string& field, const std::string& threshold) : field(field), threshold(threshold){

}


Participant* RuleBasedAssignment::choose(const Task& task, const std::vector<Participant*>& pool){
    const std::string& requiredRole = task.getRequiredRole();

    if(field != "role" && field != "maxWorkload"){
        return NULL;
    }

    int maximum = 0;

    if(field == "maxWorkload"){
        std::istringstream stream(threshold);

        if(!(stream >> maximum) || maximum < 0){
            return NULL;
        }

        stream >> std::ws;
        if(!stream.eof()){
            return NULL;
        }
    }

    for (size_t i = 0; i < pool.size(); i++){
        Participant* participant = pool[i];

        if(participant == NULL){
            continue;
        }
        if(!participant->hasRole(requiredRole)){
            continue;
        }

        if(field == "role"){
            if(participant->hasRole(threshold)){
                return participant;
            }
        }else if(field == "maxWorkload"){
            if(participant->getWorkload() <= maximum){
                return participant;
            }
        }
    }
    return NULL;
}