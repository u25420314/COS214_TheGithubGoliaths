#include "EmailAdapter.h"

bool EmailAdapter::send(const std::string& to, const std::string& subject,
                        const std::string& body) {
    return service_.sendRaw(to, subject + "\n\n" + body);  // stub wants one string
}