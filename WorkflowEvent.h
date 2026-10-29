#pragma once
#include <string>

/** Something that happened in a workflow. Published by the engine to observers. */
class WorkflowEvent {
public:
    WorkflowEvent(std::string type, std::string instanceId, std::string nodeId);

    const std::string& getType() const { return type; }
    const std::string& getInstanceId() const { return instanceId; }
    const std::string& getNodeId() const { return nodeId; }
    const std::string& getTime() const { return time; }

private:
    std::string type;
    std::string instanceId;
    std::string nodeId;
    std::string time;   // set automatically when the event is created
};