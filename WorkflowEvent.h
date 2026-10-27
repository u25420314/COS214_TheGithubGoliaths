#pragma once
#include <string>

/** An immutable record of something that happened in a workflow. Published to observers. */
class WorkflowEvent {
public:
    WorkflowEvent(std::string type, std::string instanceId, std::string nodeId,
                  std::string time = currentTimestamp());

    const std::string& getType() const { return type_; }
    const std::string& getInstanceId() const { return instanceId_; }
    const std::string& getNodeId() const { return nodeId_; }
    const std::string& getTime() const { return time_; }

    /** Local time formatted as "YYYY-MM-DD HH:MM:SS". */
    static std::string currentTimestamp();

private:
    std::string type_;
    std::string instanceId_;
    std::string nodeId_;
    std::string time_;
};