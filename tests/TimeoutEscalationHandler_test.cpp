#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "TimeoutEscalationHandler.h"
#include "ApprovalHandler.h"
#include "ApprovalRequest.h"

class TestHandler : public ApprovalHandler{
    public:
        bool called;
        TestHandler() : called(false){}

        void handle(ApprovalRequest& request) override{
            called = true;
            request.setDecision("Forwarded");

        }
};

TEST_CASE("Request escaltes when days exceed limit"){
    ApprovalRequest request(nullptr,7);
    request.setDaysWaiting(7);

    TimeoutEscalationHandler handler(5);

    handler.handle(request);

    CHECK(request.getDecision() == "Escalated");
}

TEST_CASE("Request does not escalate below limit"){
    ApprovalRequest request(nullptr, 3);
    request.setDaysWaiting(3);

    TimeoutEscalationHandler handler(5);

    handler.handle(request);

    CHECK(request.getDecision() == "pending");
}

TEST_CASE("Request does not escalate exaclty at limit"){
    ApprovalRequest request(nullptr, 5);
    request.setDaysWaiting(5);

    TimeoutEscalationHandler handler(5);

    handler.handle(request);

    CHECK(request.getDecision() == "pending");
}

TEST_CASE("Request forwards when timeout not exceeded"){
    ApprovalRequest request(nullptr, 3);
    request.setDaysWaiting(3);

    TimeoutEscalationHandler handler(5);
    TestHandler nextHandler;

    handler.setNext(&nextHandler);
    handler.handle(request);

    CHECK(nextHandler.called == true);
    CHECK(request.getDecision() == "Forwarded");
}