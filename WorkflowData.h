#ifndef TIVIDY_WORKFLOWDATA_H
#define TIVIDY_WORKFLOWDATA_H

#include <string>
#include <map>

class WorkflowData {
    map<string, string> fields;
    public:
    string get(string key);
    void set(string key, string value);
};

#endif
