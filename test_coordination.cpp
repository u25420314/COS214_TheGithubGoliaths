// Needs doctest.h on the include path and ONE file somewhere with
// #define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN (Person 5's test setup should have it).
#include "doctest.h"
#include "AuditLogger.h"
#include "EmailAdapter.h"
#include "NotificationService.h"
#include "PaymentAdapter.h"
#include "ProgressMonitor.h"
#include "WorkflowSubject.h"

namespace {
struct Recorder : WorkflowObserver {
    std::vector<std::string> types;
    void update(const WorkflowEvent& e) override { types.push_back(e.getType()); }
};
struct SelfDetacher : WorkflowObserver {
    WorkflowSubject& subject; int calls = 0;
    explicit SelfDetacher(WorkflowSubject& s) : subject(s) {}
    void update(const WorkflowEvent&) override { ++calls; subject.detach(this); }
};
struct Detaches : WorkflowObserver {   // removes another observer when called
    WorkflowSubject& subject; WorkflowObserver* victim;
    Detaches(WorkflowSubject& s, WorkflowObserver* v) : subject(s), victim(v) {}
    void update(const WorkflowEvent&) override { subject.detach(victim); }
};
WorkflowEvent ev(const char* type) { return WorkflowEvent(type, "inst1", "taskA"); }
}  // namespace

TEST_CASE("Observers receive events") {
    WorkflowSubject s; Recorder a, b;
    s.attach(&a); s.attach(&b); s.attach(&a);  // duplicate ignored
    CHECK(s.observerCount() == 2);
    s.notifyObservers(ev("task_completed"));
    CHECK(a.types == std::vector<std::string>{"task_completed"});
    CHECK(b.types.size() == 1);
    s.detach(&b);
    s.notifyObservers(ev("task_assigned"));
    CHECK(a.types.size() == 2);
    CHECK(b.types.size() == 1);
}

TEST_CASE("Detaching during notification is safe") {
    WorkflowSubject s; Recorder r;
    SelfDetacher self(s);
    s.attach(&self); s.attach(&r);
    s.notifyObservers(ev("x")); s.notifyObservers(ev("x"));
    CHECK(self.calls == 1);
    CHECK(r.types.size() == 2);

    WorkflowSubject s2; Recorder victim;
    Detaches killer(s2, &victim);
    s2.attach(&killer); s2.attach(&victim);
    s2.notifyObservers(ev("x"));
    CHECK(victim.types.empty());   // removed before its turn
}

TEST_CASE("AuditLogger stores history") {
    AuditLogger log;
    log.update(ev("task_assigned"));
    log.update(ev("task_completed"));
    REQUIRE(log.getEntries().size() == 2);
    CHECK(log.getEntries()[0].find("task_assigned") != std::string::npos);
    CHECK(log.getEntries()[1].find("node=taskA") != std::string::npos);
}

TEST_CASE("ProgressMonitor counts completions and undos") {
    ProgressMonitor m(4);
    m.update(ev("task_completed")); m.update(ev("task_completed"));
    CHECK(m.percentage() == 50);
    m.update(ev("task_undone"));
    CHECK(m.getCompleted() == 1);
    m.update(ev("task_undone")); m.update(ev("task_undone"));
    CHECK(m.getCompleted() == 0);   // never negative
    CHECK(m.report() == "Progress: 0/4 tasks complete (0%)");
}

TEST_CASE("Adapters work with the stubs") {
    EmailServiceStub mail; EmailAdapter email(mail);
    CHECK(email.send("a@b.c", "Hi", "Body"));
    CHECK(mail.sentCount == 1);
    CHECK(mail.lastText == "Hi\n\nBody");
    CHECK_FALSE(email.send("", "Hi", "Body"));

    PaymentServiceStub bank; PaymentAdapter pay(bank);
    CHECK(pay.pay("ACC-1", 12.34));
    CHECK(bank.lastCents == 1234);
    CHECK(pay.lastTransactionId() > 0);
    CHECK_FALSE(pay.pay("ACC-1", 0));
}

TEST_CASE("NotificationService sends only important events") {
    EmailServiceStub mail; EmailAdapter email(mail);
    NotificationService n(&email);
    n.update(ev("workflow_started"));      // ignored
    n.update(ev("task_assigned"));
    n.update(ev("workflow_completed"));
    CHECK(n.getMessages().size() == 2);
    CHECK(mail.sentCount == 2);
}