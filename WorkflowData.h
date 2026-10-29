#ifndef TIVIDY_WORKFLOWDATA_H
#define TIVIDY_WORKFLOWDATA_H

#include <map>
#include <string>

class WorkflowData {
public:
    void set(const std::string& key, const std::string& value);

    std::string get(const std::string& key) const;
    bool has(const std::string& key) const;

private:
    std::map<std::string, std::string> fields;
};

#endif
