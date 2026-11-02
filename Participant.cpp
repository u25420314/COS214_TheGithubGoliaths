#include "Participant.h"

#include <string>
#include <vector>

Participant::Participant(const std::string& id, const std::string& name, const std::vector<std::string>& roles) : id(id), name(name), roles(roles), workload(0){

}

std::string Participant::getId() const{
    return id;
}

std::string Participant::getName() const{
    return name;
}

std::vector<std::string> Participant::getRoles() const{
    return roles;
}

int Participant::getWorkload() const{
    return workload;
}

bool Participant::hasRole(const std::string& role) const{
    for(size_t i = 0; i < roles.size(); i++){
        if(roles[i] == role){
            return true;
        }
    }
    return false;
}



void Participant::incrementWorkload(){
    workload++;
}

void Participant::decrementWorkload(){
    if(workload > 0){
        workload--;
    }
}