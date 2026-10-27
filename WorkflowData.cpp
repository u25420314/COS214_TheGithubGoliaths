#include "WorkflowData.h"

#include <map>
#include <string>

std::string WorkflowData::get(std::string key){
    return fields.find(key);
}

void WorkflowData::set(std::string key, std::string value){
    fields.insert({key, value});
}
