#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "RoleApprover.h"
#include "ApprovalHandler.h"
#include "ApprovalRequest.h"

class TestHandler : public ApprovalHandler{
    public:
        bool called;
        testHandler() : called(false){}
        void handle(ApprovalRequest& request) override{
            called = true;
            request.setDecision("Forwarded");
        }
};

TEST_CASE("RoleApprover handles a request without a task"){
    ApprovalRequest request(nullptr);
    RoleApprover approver("Admin");

    approver.handle(request);

    CHECK(request.getDecision() == "pending");
}

TEST_CASE("RoleApprover forwards ri the next handler"){
    ApprovalRequest request(nullptr);
    RoleApprover approver("Admin");
    TestHandler nextHandler;

    approver.setNext(&nextHandler);
    approver.handle(request);

    CHECK(nextHandler.called == true);
    CHECK(request.getDecision() == "Forwarded");
}

TEST_CASE("RoleApprover does not forward without a successor"){
    ApprovalRequest request(nullptr);
    RoleApprover approver("Lecturer");
    TestHandler last;

    first.setNext(&second);
    second.setNext(&last);

    first.handle(request);

    CHECK(last.called == true);
    CHECK(request.getDecision() == "Forwarded");
}