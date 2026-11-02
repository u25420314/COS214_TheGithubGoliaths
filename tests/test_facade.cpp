#include "doctest.h"

#include "TividySystem.h"
#include "StandardWorkflowBuilder.h"
#include "WorkflowDefinition.h"
#include "Participant.h"

#include <string>

namespace {
    struct FacadeFixture {
        TividySystem system{ { Participant("p1", "Nathan", { "clerk" }) } };

        FacadeFixture() {
            StandardWorkflowBuilder b;

            b.reset();
            b.setWorkflowName("Test");
            b.beginStage("Main", false);
            b.addTask("Verify ID", "clerk");
            b.addTask("Review", "clerk");
            b.endStage();

            system.registerDefinition(new WorkflowDefinition(b.getResult()));
        }

        bool historyHas(const std::string& text) {
            
            for(const std::string& line : system.getHistory()) {
                if(line.find(text) != std::string::npos) return true;
            }

            return false;
        }

        int historyCount(const std::string& text) {
            int n = 0;

            for(const std::string& line : system.getHistory()) {
                if(line.find(text) != std::string::npos) n++;
            }

            return n;
        }
    };
}

TEST_CASE_FIXTURE(FacadeFixture, "When workflow starts only one task assigned") {
    system.startWorkflow("Test", WorkflowData());

    CHECK(historyCount("task_assigned") == 1);
}

TEST_CASE_FIXTURE(FacadeFixture, "Nothing starts when an unknown workflow is started, its reported instead") {
    std::string id = system.startWorkflow("Test123", WorkflowData());
    
    CHECK(id == "Definition not found");
    CHECK_FALSE(historyHas("workflow_started"));
}

TEST_CASE_FIXTURE(FacadeFixture, "Starting same workflow twice creates 2 seperate instances") {
    std::string id1 = system.startWorkflow("Test", WorkflowData());
    std::string id2 = system.startWorkflow("Test", WorkflowData());

    CHECK(id1 != id2);
}

TEST_CASE_FIXTURE(FacadeFixture, "When workflow starts it returns an id") {
    std::string id = system.startWorkflow("Test", WorkflowData());

    CHECK_FALSE(id.empty());
}

TEST_CASE_FIXTURE(FacadeFixture, "next task assigned when task completed") {
    system.startWorkflow("Test", WorkflowData());
    system.startTask("n2");
    system.completeTask("n2");

    CHECK(historyCount("task_assigned") == 2);
}

TEST_CASE_FIXTURE(FacadeFixture, "Completing the last task completes the workflow") {
    system.startWorkflow("Test", WorkflowData());
    system.startTask("n2");
    system.completeTask("n2");
    system.startTask("n3");
    system.completeTask("n3");

    CHECK(historyHas("instance_completed"));
}

TEST_CASE_FIXTURE(FacadeFixture, "Reserve last command with undo in facade") {
    system.startWorkflow("Test", WorkflowData());
    system.startTask("n2");
    system.completeTask("n2");

    system.undoLast();

    CHECK(historyHas("undone"));
    CHECK_NOTHROW(system.completeTask("n2"));
}

TEST_CASE_FIXTURE(FacadeFixture, "Rejected tasks can be assigned to someone else") {
    Participant other("p2", "Nathan", { "clerk" });

    system.startWorkflow("Test", WorkflowData());
    system.startTask("n2");
    system.rejectTask("n2");

    CHECK(historyHas("rejected"));
    CHECK_NOTHROW(system.assignTask("n2", &other));
}

TEST_CASE_FIXTURE(FacadeFixture, "Started workflow, recorded in history") {
    system.startWorkflow("Test", WorkflowData());

    CHECK(historyHas("workflow_started"));
}