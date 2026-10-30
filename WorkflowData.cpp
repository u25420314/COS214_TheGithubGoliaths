#include "WorkflowData.h"

//stores a value under a key
void WorkflowData::set(const std::string& key, const std::string& value) {
    fields[key] = value;
}

//returns the value or an empty string if the key is missing
std::string WorkflowData::get(const std::string& key) const {
    std::map<std::string, std::string>::const_iterator it = fields.find(key);
    if (it == fields.end()) {
        return std::string();
    }
    return it->second;
}

//says whether a key exists
bool WorkflowData::has(const std::string& key) const {
    return fields.find(key) != fields.end();
}
