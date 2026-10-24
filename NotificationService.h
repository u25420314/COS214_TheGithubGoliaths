#pragma once
#include <string>
#include <vector>
#include "WorkflowObserver.h"
#include "EmailAdapter.h"

/** Observer that turns important events into messages (sent through a MessageSender if given). */
class NotificationService : public WorkflowObserver {
public:
    explicit NotificationService(MessageSender* sender = nullptr,
                                 std::string recipient = "team@tividy.local");
    void update(const WorkflowEvent& e) override;
    const std::vector<std::string>& getMessages() const { return messages_; }

private:
    MessageSender* sender_;
    std::string recipient_;
    std::vector<std::string> messages_;
};