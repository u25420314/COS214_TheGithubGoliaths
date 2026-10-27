// Depends on Person 3/4's classes, so it only compiles once they are merged.
#include "doctest.h"
#include "Participant.h"
#include "ProgressMonitor.h"
#include "StandardWorkflowBuilder.h"
#include "Task.h"
#include "TaskState.h"
#include "WorkNode.h"
#include "WorkNodeIterator.h"
#include "WorkflowData.h"
#include "WorkflowEngine.h"
#include "WorkflowInstance.h"

namespace {
Task* findTask(WorkflowInstance& inst, const std::string& name) {
    auto it = inst.getRoot()->createIterator();
    for (it->first(); !it->isDone(); it->next())
        if (it->current()->getName() == name) return dynamic_cast<Task*>(it->current());
    return nullptr;
}
std::string stateOf(Task* t) { return t->getState()->getName(); }
}  // namespace

TEST_CASE("The next task activates only when its dependencies are complete") {
    StandardWorkflowBuilder b;
    b.reset();
    b.beginStage("main", false);          // sequential: A then B
    b.addTask("A", "clerk");
    b.addTask("B", "clerk");
    b.endStage();
    WorkflowDefinition def = b.getResult();

    WorkflowEngine engine({Participant("p1", "Alice", {"clerk"})});
    ProgressMonitor monitor(2);
    engine.attach(&monitor);

    WorkflowInstance& inst = engine.createInstance(def, WorkflowData());
    Task* a = findTask(inst, "A");
    Task* bTask = findTask(inst, "B");
    REQUIRE(a); REQUIRE(bTask);

    CHECK(stateOf(a) == "Assigned");
    CHECK(stateOf(bTask) == "Available");   // blocked by A

    a->start();
    a->complete();                          // Task tells the engine via notify()
    CHECK(stateOf(bTask) == "Assigned");
    CHECK(monitor.getCompleted() == 1);
}