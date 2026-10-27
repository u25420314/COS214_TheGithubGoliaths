#include "NotificationService.h"

NotificationService::NotificationService(MessageSender* sender, std::string recipient)
    : sender_(sender), recipient_(std::move(recipient)) {}

void NotificationService::update(const WorkflowEvent& e) {
    const std::string& t = e.getType();
    std::string text;
    if (t == "task_assigned")           text = "Task " + e.getNodeId() + " has been assigned.";
    else if (t == "task_rejected")      text = "Task " + e.getNodeId() + " was rejected.";
    else if (t == "task_escalated")     text = "Task " + e.getNodeId() + " was escalated.";
    else if (t == "task_unassignable")  text = "No one available for task " + e.getNodeId() + ".";
    else if (t == "workflow_completed") text = "Workflow " + e.getInstanceId() + " is complete.";
    else return;  // not worth a notification

    messages_.push_back(text);
    if (sender_) sender_->send(recipient_, "TIVIDY: " + t, text);
}