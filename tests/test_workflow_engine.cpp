// Needs Person 3/4/5's classes, so it only builds once their code is merged.
#include "doctest.h"
#include "Participant.h"
#include "StandardWorkflowBuilder.h"
#include "Task.h"
#include "WorkNode.h"
#include "WorkNodeIterator.h"
#include "WorkflowData.h"
#include "WorkflowEngine.h"
#include "WorkflowInstance.h"

namespace {
struct Recorder : WorkflowObserver {
    std::vector<std::string> types;
    void update(const WorkflowEvent& e) override { types.push_back(e.getType()); }
    int count(const std::string& t) const { return (int)std::count(types.begin(), types.end(), t); }
};
// The n-th Task in the instance's tree (0 = first).
Task* nthTask(WorkflowInstance* inst, int n) {
    auto it = inst->getRoot()->createIterator();
    for (it->first(); !it->isDone(); it->next())
        if (Task* t = dynamic_cast<Task*>(it->current())) if (n-- == 0) return t;
    return nullptr;
}
}  // namespace

TEST_CASE("The next task activates only when its dependencies are complete") {
    StandardWorkflowBuilder b;
    b.reset();
    b.beginStage("main", false);     // sequential: A then B
    b.addTask("A", "clerk");
    b.addTask("B", "clerk");
    b.endStage();
    WorkflowDefinition def = b.getResult();

    WorkflowEngine engine({Participant("p1", "Alice", {"clerk"})});
    Recorder rec; engine.attach(&rec);

    WorkflowInstance* inst = engine.createInstance(def, WorkflowData());
    Task* a = nthTask(inst, 0); Task* bTask = nthTask(inst, 1);
    REQUIRE(a); REQUIRE(bTask);
    CHECK(rec.count("task_assigned") == 1);   // only A so far

    a->start(); a->complete();                // Task reports "completed" to the engine
    CHECK(rec.count("task_assigned") == 2);   // now B
    CHECK(rec.count("instance_completed") == 0);

    bTask->start(); bTask->complete();
    CHECK(rec.count("instance_completed") == 1);
}