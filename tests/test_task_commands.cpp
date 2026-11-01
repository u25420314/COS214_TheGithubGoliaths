#include "doctest.h"

#include "CommandHistory.h"
#include "AssignTaskCommand.h"
#include "StartTaskCommand.h"
#include "CompleteTaskCommand.h"
#include "RejectTaskCommand.h"
#include "Task.h"
#include "TaskState.h"
#include "Participant.h"

#include <string>

namespace {
    struct TaskFixture {
        Participant Nathan{"p1", "Nathan", {"clerk"}};
        Task task{"T1", "Verify ID", "clerk"};
        CommandHistory history;

        std::string state() { return task.getState()->getName(); }
    };
}

//execute
TEST_CASE_FIXTURE(TaskFixture, "AssignTaskCommand assigns a task to a participant") {
    history.executeCommand(new AssignTaskCommand(nullptr, &task, &Nathan));

    CHECK(state() == "Assigned");
    CHECK(task.getAssignee() == &Nathan);
}

TEST_CASE_FIXTURE(TaskFixture, "StartTaskCommand moves an assigned task to InProgress") {
    history.executeCommand(new AssignTaskCommand(nullptr, &task, &Nathan));
    history.executeCommand(new StartTaskCommand(nullptr, &task));

    CHECK(state() == "InProgress");
}

TEST_CASE_FIXTURE(TaskFixture, "CompleteTaskCommand completes a started task") {
    history.executeCommand(new AssignTaskCommand(nullptr, &task, &Nathan));
    history.executeCommand(new StartTaskCommand(nullptr, &task));
    history.executeCommand(new CompleteTaskCommand(nullptr, &task, &Nathan));

    CHECK(state() == "Completed");
    CHECK(task.getAssignee() == &Nathan);
}

TEST_CASE_FIXTURE(TaskFixture, "RejectTaskCommand completes a started task") {
    history.executeCommand(new AssignTaskCommand(nullptr, &task, &Nathan));
    history.executeCommand(new StartTaskCommand(nullptr, &task));
    history.executeCommand(new RejectTaskCommand(nullptr, &task));

    CHECK(state() == "Rejected");
}

//undo
TEST_CASE_FIXTURE(TaskFixture, "Undoing a AssignTaskCommand restores the task back Available with no assignee") {
    history.executeCommand(new AssignTaskCommand(nullptr, &task, &Nathan));
    history.undo();

    CHECK(state() == "Available");
    CHECK(task.getAssignee() == nullptr);
}

TEST_CASE_FIXTURE(TaskFixture, "Undoing a CompleteTaskCommand restores the task back Available with no assignee") {
    history.executeCommand(new AssignTaskCommand(nullptr, &task, &Nathan));
    history.executeCommand(new StartTaskCommand(nullptr, &task));
    history.executeCommand(new CompleteTaskCommand(nullptr, &task, &Nathan));
    history.undo();

    CHECK(state() == "InProgress");
}

//error checking
TEST_CASE_FIXTURE(TaskFixture, "Completing a task that hasnt started is refused") {
    history.executeCommand(new AssignTaskCommand(nullptr, &task, &Nathan));

    CHECK_THROWS(history.executeCommand(new CompleteTaskCommand(nullptr, &task, &Nathan)));
    CHECK(state() == "Assigned");
}

TEST_CASE_FIXTURE(TaskFixture, "Starting a task with nobody assigned does nothing") {
    history.executeCommand(new StartTaskCommand(nullptr, &task));

    CHECK(state() == "Available");
}

TEST_CASE_FIXTURE(TaskFixture, "A task stays the same after an action is refused") {
    history.executeCommand(new AssignTaskCommand(nullptr, &task, &Nathan));

    std::string OGstate = state();
    Participant* OGassignee = task.getAssignee();
    
    CHECK_THROWS(history.executeCommand(new CompleteTaskCommand(nullptr, &task, &Nathan)));

    CHECK(OGstate == state());
    CHECK(OGassignee == task.getAssignee());
}