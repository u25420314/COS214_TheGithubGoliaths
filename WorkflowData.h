#ifndef TIVIDY_WORKFLOWDATA_H
#define TIVIDY_WORKFLOWDATA_H

#include <map>
#include <string>

/// @brief The class for WorkflowData
class WorkflowData {
public:
    /// @brief sets the data for the workflow
    /// @param key The key
    /// @param value The value
    void set(const std::string& key, const std::string& value);

    /// @brief get the workflow data
    /// @param key the key of the data
    /// @return the value of the data
    std::string get(const std::string& key) const;

    /// @brief data contains the data at key
    /// @param key the key of the data
    /// @return if the data contains the key or not
    bool has(const std::string& key) const;

private:
    /// @brief the fields of the data
    std::map<std::string, std::string> fields;
};

#endif
