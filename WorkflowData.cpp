#include "WorkflowData.h"

void WorkflowData::set(const std::string& key, const std::string& value) {
    fields[key] = value;
}

std::string WorkflowData::get(const std::string& key) const {
    std::map<std::string, std::string>::const_iterator it = fields.find(key);
    if (it == fields.end()) {
        return std::string();
    }
    return it->second;
}

bool WorkflowData::has(const std::string& key) const {
    return fields.find(key) != fields.end();
}
