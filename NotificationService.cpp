#include "NotificationService.h"

void NotificationService::update(const WorkflowEvent& e) {
    const std::string& t = e.getType();

    if (t == "task_assigned") out << "[Notification] Task " << e.getNodeId() << " has been assigned.\n";
    else if (t == "rejected") out << "[Notification] Task " << e.getNodeId() << " was rejected.\n";
    else if (t == "escalated") out << "[Notification] Task " << e.getNodeId() << " was escalated.\n";
    else if (t == "instance_completed") out << "[Notification] Workflow " << e.getInstanceId() << " is complete.\n";
}