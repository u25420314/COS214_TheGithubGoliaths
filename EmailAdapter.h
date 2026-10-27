#pragma once
#include <string>
#include "StubServices.h"

/** Target interface the rest of the system uses to send messages. */
class MessageSender {
public:
    virtual ~MessageSender() = default;
    virtual bool send(const std::string& to, const std::string& subject,
                      const std::string& body) = 0;
};

/** Adapter: MessageSender -> EmailServiceStub::sendRaw. */
class EmailAdapter : public MessageSender {
public:
    explicit EmailAdapter(EmailServiceStub& service) : service_(service) {}
    bool send(const std::string& to, const std::string& subject,
              const std::string& body) override;
private:
    EmailServiceStub& service_;
};