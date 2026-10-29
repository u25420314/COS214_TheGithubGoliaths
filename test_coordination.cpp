// Observer-side tests. They use the engine only as the Subject, so no workflow is needed.
#include "doctest.h"
#include <sstream>
#include "AuditLogger.h"
#include "NotificationService.h"
#include "ProgressMonitor.h"
#include "WorkflowEngine.h"

namespace {
struct Recorder : WorkflowObserver {
    std::vector<std::string> types;
    void update(const WorkflowEvent& e) override { types.push_back(e.getType()); }
};
struct SelfDetacher : WorkflowObserver {
    WorkflowEngine& engine; int calls = 0;
    explicit SelfDetacher(WorkflowEngine& e) : engine(e) {}
    void update(const WorkflowEvent&) override { ++calls; engine.detach(this); }
};
struct Detacher : WorkflowObserver {   // removes another observer when called
    WorkflowEngine& engine; WorkflowObserver* victim;
    Detacher(WorkflowEngine& e, WorkflowObserver* v) : engine(e), victim(v) {}
    void update(const WorkflowEvent&) override { engine.detach(victim); }
};
WorkflowEvent ev(const char* type) { return WorkflowEvent(type, "inst1", "taskA"); }
}  // namespace

TEST_CASE("Observers receive events") {
    WorkflowEngine engine; Recorder a, b;
    engine.attach(&a); engine.attach(&b); engine.attach(&a);   // duplicate ignored
    engine.notifyObservers(ev("completed"));
    CHECK(a.types == std::vector<std::string>{"completed"});
    CHECK(b.types.size() == 1);

    engine.detach(&b);
    engine.notifyObservers(ev("task_assigned"));
    CHECK(a.types.size() == 2);
    CHECK(b.types.size() == 1);   // detached, hears nothing more
}

TEST_CASE("Detaching during notification is safe") {
    WorkflowEngine engine; Recorder r; SelfDetacher self(engine);
    engine.attach(&self); engine.attach(&r);
    engine.notifyObservers(ev("x")); engine.notifyObservers(ev("x"));
    CHECK(self.calls == 1);
    CHECK(r.types.size() == 2);

    WorkflowEngine engine2; Recorder victim; Detacher killer(engine2, &victim);
    engine2.attach(&killer); engine2.attach(&victim);
    engine2.notifyObservers(ev("x"));
    CHECK(victim.types.empty());   // removed before its turn
}

TEST_CASE("AuditLogger stores history") {
    AuditLogger log;
    log.update(ev("task_assigned")); log.update(ev("completed"));
    REQUIRE(log.getEntries().size() == 2);
    CHECK(log.getEntries()[0].find("task_assigned") != std::string::npos);
    CHECK(log.getEntries()[1].find("node taskA") != std::string::npos);
}

TEST_CASE("ProgressMonitor counts completed and undone") {
    ProgressMonitor m;
    m.update(ev("completed")); m.update(ev("completed"));
    CHECK(m.report() == "2 task(s) completed");
    m.update(ev("undone"));
    CHECK(m.getCompletedCount() == 1);
    m.update(ev("undone")); m.update(ev("undone"));
    CHECK(m.getCompletedCount() == 0);   // never negative
}

TEST_CASE("NotificationService only reports important events") {
    std::ostringstream out; NotificationService n(out);
    n.update(ev("instance_created"));   // ignored
    CHECK(out.str().empty());
    n.update(ev("task_assigned"));
    n.update(ev("instance_completed"));
    CHECK(out.str().find("has been assigned") != std::string::npos);
    CHECK(out.str().find("is complete") != std::string::npos);
}