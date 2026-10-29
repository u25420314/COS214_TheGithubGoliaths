#ifndef TIVIDY_WORKFLOWDATA_H
#define TIVIDY_WORKFLOWDATA_H

#include <string>
#include <map>

class WorkflowData {
    std::map<std::string, std::string> fields;
    public:
    std::string get(std::string key);
    void set(std::string key, std::string value);
};

#endif
