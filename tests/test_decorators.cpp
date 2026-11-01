#include "doctest.h"
#include <memory>
#include <string>
#include <vector>
#include "WorkNode.h"
#include "WorkflowMediator.h"
#include "WorkflowData.h"
#include "Task.h"
#include "PriorityDecorator.h"
#include "DeadlineDecorator.h"
#include "ValidationDecorator.h"
#include <stdexcept>

namespace{

    //a plain lead node
    class FakeNode : public WorkNode {
        public:
            static int alive;
            static int totalWork;
            int work;
            int finished;
            bool startable;
            Task* item;

            
            explicit FakeNode(bool ok = true)
            : WorkNode("n", "Node"), work(0), finished(0), startable(ok), item(nullptr){
                ++alive;
            }

            ~FakeNode() override{
                    --alive;
            }


            WorkNode* clone() const override { 
                return new FakeNode(startable); 
            }
            WorkNodeIterator* createIterator() override {
                 return nullptr; 
            }

            bool isComplete() const override { 
                return work > 0;
             }
            bool canStart() const override { 
                return startable; 
            }
            void collectReady(std::vector<Task*>& out) override { 
                if (item) out.push_back(item); 
            }

        protected:
            void doWork() override{
                ++work;
                ++totalWork;
            }

            void finish() override {
                ++finished;
                WorkNode::finish();
            }
        };

        int FakeNode::alive = 0;
        int FakeNode::totalWork = 0;


    class CountingMediator : public WorkflowMediator{
        public:
            int notifications;
            CountingMediator() : notifications(0){}
            void notify(WorkNode*, const std::string&) override { ++notifications;}
    };


    std::vector<std::string> fields() {
        std::vector<std::string> f;
        f.push_back("policyNumber");
        f.push_back("claimAmount");
        return f;
    }

    void fill(WorkflowData& d){
        d.set("policyNumber", "POL-1");
        d.set("claimAmount", "100");
    }
}

//all the task behaviour for decoratior. shown with priority decorator(the simoplest one)

TEST_CASE("a decorator takes te id name of the node and wraps it"){
    PriorityDecorator p(new FakeNode(), 1);
    CHECK_EQ(p.getName(), "Node");
    CHECK_EQ(p.getId(), "n");
}

TEST_CASE("a decorator exposes the wrapped node as its only child"){
    FakeNode* leaf = new FakeNode();
    PriorityDecorator p(leaf ,1 );
    std::vector<WorkNode*> kids = p.getChildren();
    REQUIRE_EQ(kids.size(), 1u);
    CHECK(kids[0] == leaf);
}


TEST_CASE(" a decorator with nothinf wrapped cannot start and does not crash"){
    PriorityDecorator p(nullptr, 1); //wrapped node and level - wrapped = null therefor nothing wrapped
    CHECK(p.getChildren().empty());
    CHECK_FALSE(p.canStart());
    CHECK_FALSE(p.isComplete());
    p.run();


}

TEST_CASE("a decorator forwards canStart, isCOmplete and the work"){
    FakeNode* leaf = new FakeNode();
    PriorityDecorator p(leaf, 1);
    CHECK(p.canStart());
    CHECK_FALSE(p.isComplete());
    p.run();
    CHECK_EQ(leaf->work,1 );
    CHECK(p.isComplete());
}


TEST_CASE("a decorator does not start when the wrapped node annot"){
    FakeNode* leaf = new FakeNode(false);
    PriorityDecorator p(leaf, 1);
    CHECK_FALSE(p.canStart());
    p.run();
    CHECK_EQ(leaf->work, 0);
}

TEST_CASE("setMediator reaches the wrapped node too"){
    FakeNode* leaf = new FakeNode();
    PriorityDecorator p(leaf, 1);
    CountingMediator m;
    p.setMediator(&m);
    CHECK(p.getMediator() == &m);
    CHECK(leaf->getMediator() == &m);
    

}

TEST_CASE("destroying a decorator destroys the node that it wraps"){
    int before = FakeNode::alive;
    {
        PriorityDecorator p(new DeadlineDecorator(new FakeNode(), 3), 1);
        CHECK_EQ(FakeNode::alive, before + 1);
    }

    CHECK_EQ(FakeNode::alive, before);
}

TEST_CASE("a decorated node reports completion to the mediator exactly once"){
    FakeNode* leaf = new FakeNode();
    PriorityDecorator p(leaf, 1);
    CountingMediator m;
    p.setMediator(&m);
    p.run();
    CHECK_EQ(leaf->finished, 1);
    CHECK_EQ(m.notifications, 1);

}



//priority decorator

TEST_CASE("Prioritydecorator stores and changes its level"){
    PriorityDecorator p(new FakeNode(), 3);
    CHECK_EQ(p.getLevel(), 3);
    p.setLevel(5);
    CHECK_EQ(p.getLevel(), 5);

}

TEST_CASE("collectReady forwards the wrapped nodes read tasks"){
    Task t("t1", "Task", "Role");
    FakeNode* leaf = new FakeNode();
    leaf->item = &t;
    DeadlineDecorator d(leaf, 5); //a decorator that does not re order
    std::vector<Task*> out;
    d.collectReady(out);
    REQUIRE_EQ(out.size(), 1u);
    CHECK(out[0] == &t);
}


TEST_CASE("PriorityDecorator puts its ready tasks ahead of those already collected"){
    Task plain("p", "Plain", "Role");
    Task urgent("u", "Urgent", "Role");
    FakeNode* leaf = new FakeNode();
    leaf->item = &urgent;
    PriorityDecorator p(leaf, 5);


    std::vector<Task*> out;
    out.push_back(&plain);
    p.collectReady(out);
    REQUIRE_EQ(out.size(), 2u);
    CHECK(out[0] == &urgent);
    CHECK(out[1] == &plain);
}


//validation decorator

TEST_CASE("Validation blocks work while required fields are missing"){
    FakeNode* leaf = new FakeNode();
    ValidationDecorator v(leaf, fields());
    WorkflowData d;
    v.bindData(&d);
    v.run();
    CHECK_EQ(leaf->work, 0);
    CHECK_EQ(v.missingFields().size(), 2u);
    d.set("policyNumber", "POL-1");
    REQUIRE_EQ(v.missingFields().size(), 1u);
    CHECK_EQ(v.missingFields()[0], "claimAmount");
}


TEST_CASE("ValidatioNDecoraotor allows work once all fields are filled"){
    FakeNode* leaf = new FakeNode();
    ValidationDecorator v(leaf, fields());
    WorkflowData d;
    v.bindData(&d);
    fill(d);
    v.run();
    CHECK_EQ(leaf->work, 1);
    CHECK(v.missingFields().empty());

}

TEST_CASE("an empty field value counts as missing"){
     FakeNode* leaf = new FakeNode();
    ValidationDecorator v(leaf, fields());
    WorkflowData d;
    fill(d);
    d.set("claimAmount", "");
    v.bindData(&d);
    v.run();
    CHECK_EQ(leaf->work, 0);
}

TEST_CASE("an unbound validationDecorator fails safe and blocks"){
    FakeNode* leaf = new FakeNode();
    ValidationDecorator v(leaf, fields());
    v.run();
    CHECK_EQ(leaf->work, 0);
    CHECK_EQ(v.missingFields().size(), 2u);
}


TEST_CASE("ValidationDecorator also respects the wrapped node's own canStart"){
    FakeNode* leaf = new FakeNode(false);
    ValidationDecorator v(leaf, fields());
    WorkflowData d;
    fill(d);
    v.bindData(&d);
    v.run();
    CHECK_EQ(leaf->work, 0);
}


//deadline decorator

TEST_CASE("DeadlineDecorator is overdue after the due day has passed"){
    DeadlineDecorator d1(new FakeNode(), 5);
    CHECK_FALSE(d1.isOverdue());
    d1.advanceDays(5);
    CHECK_FALSE(d1.isOverdue());
    d1.advanceDays(1);
    CHECK(d1.isOverdue());
    CHECK_EQ(d1.daysElapsed(), 6);
    CHECK_EQ(d1.getDueDays(), 5);


}

TEST_CASE("advanceDays accumulates"){
    DeadlineDecorator d1(new  FakeNode(), 5);
    d1.advanceDays(2);
    d1.advanceDays(3);
    CHECK_EQ(d1.daysElapsed(), 5);

}

TEST_CASE("DeadlineDecorator never vlocks work but records lateness"){
    FakeNode* a = new FakeNode();
    DeadlineDecorator onTime(a, 5);
    onTime.advanceDays(2);
    onTime.run();
    CHECK_EQ(a->work, 1);
    CHECK_FALSE(onTime.completedLate());

    FakeNode* b = new FakeNode();
    DeadlineDecorator late(b, 5);
    late.advanceDays(9);
    late.run();
    CHECK_EQ(b->work, 1);
    CHECK(late.completedLate());
}

TEST_CASE("DeadlineDecorator rejects bad arguments"){
    CHECK_THROWS_AS(DeadlineDecorator(new FakeNode(), -1), std::invalid_argument);
    DeadlineDecorator d1(new FakeNode(), 2);
    CHECK_THROWS_AS(d1.advanceDays(-1), std::invalid_argument);
    CHECK_EQ(d1.daysElapsed(), 0);


}


//stacking

TEST_CASE("deacortoes stack"){
    FakeNode* leaf = new FakeNode();
    ValidationDecorator* v = new ValidationDecorator(leaf, fields());
    WorkflowData d;
    v->bindData(&d);
    PriorityDecorator top(new DeadlineDecorator(v, 5) , 2);

    top.run();
    CHECK_EQ(leaf->work, 0);
    CHECK_FALSE(top.canStart());
    fill(d);
    CHECK(top.canStart());
    top.run();
    CHECK_EQ(leaf->work, 1);
}


//cloning
TEST_CASE("clone is a deep copy; running the clone does not touch the orignal"){
    FakeNode* leaf = new FakeNode();
    PriorityDecorator original(new DeadlineDecorator(leaf, 4), 7);
    int before = FakeNode::alive;


    std::unique_ptr<WorkNode> copy(original.clone());
    CHECK_EQ(FakeNode::alive, before + 1);
    int work = FakeNode::totalWork;
    copy->run();
    CHECK_EQ(FakeNode::totalWork, work + 1);
    CHECK_EQ(leaf->work, 0);

    PriorityDecorator* p = dynamic_cast<PriorityDecorator*>(copy.get());
    REQUIRE(p != nullptr);
    CHECK_EQ(p->getLevel(), 7);


}

TEST_CASE("a cloned deadline decorator keeps the limit but restarts the clock"){
    DeadlineDecorator original(new FakeNode(), 5);
    original.advanceDays(10);
    std::unique_ptr<WorkNode> raw(original.clone());
    DeadlineDecorator* copy = dynamic_cast<DeadlineDecorator*>(raw.get());
    REQUIRE(copy != nullptr);
    CHECK_EQ(copy->getDueDays(), 5);
    CHECK_EQ(copy->daysElapsed(), 0);
    CHECK_FALSE(copy->isOverdue());
    CHECK(original.isOverdue());

}


TEST_CASE("a cloned ValidationDecorator is unbound until the new instance binds its own data") {
    FakeNode* leaf = new FakeNode();
    ValidationDecorator original(leaf, fields());
    WorkflowData d;
    fill(d);
    original.bindData(&d);

    std::unique_ptr<WorkNode> raw(original.clone());
    ValidationDecorator* copy = dynamic_cast<ValidationDecorator*>(raw.get());
    REQUIRE(copy != nullptr);
    int work = FakeNode::totalWork;
    copy->run();
    CHECK_EQ(FakeNode::totalWork, work);

    WorkflowData mine;
    fill(mine);
    copy->bindData(&mine);
    copy->run();
    CHECK_EQ(FakeNode::totalWork, work + 1);
}


TEST_CASE("a stack of decorators clones with every layer intact") {
    PriorityDecorator original(new DeadlineDecorator(new ValidationDecorator(new FakeNode(), fields()), 3), 9);
    std::unique_ptr<WorkNode> raw(original.clone());
    PriorityDecorator* top = dynamic_cast<PriorityDecorator*>(raw.get());
    REQUIRE(top != nullptr);
    CHECK_EQ(top->getLevel(), 9);
    std::vector<WorkNode*> l1 = top->getChildren();
    REQUIRE_EQ(l1.size(), 1u);
    CHECK(dynamic_cast<DeadlineDecorator*>(l1[0]) != nullptr);
    std::vector<WorkNode*> l2 = l1[0]->getChildren();
    REQUIRE_EQ(l2.size(), 1u);
    CHECK(dynamic_cast<ValidationDecorator*>(l2[0]) != nullptr);
}



