#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "ApprovalRequest.h"
#include <string>
#include "Task.h"

TEST_CASE("Approval requests getters and setters"){
    

    ApprovalRequest req1(nullptr, 0);

    CHECK(req1.getTask() == nullptr);
    CHECK(req1.getDecision() == "pending");
    CHECK(req1.getDaysWaiting() == 0);

    req1.setDecision("Accepted");

    CHECK(req1.getDecision() == "Accepted");


}