// COMPOSITE PATTERN - structure tests.
// The Composite treats one thing (a Task) and a group of things the exact same as a WorkNode, testing that logic

#include "doctest.h"
#include <memory>
#include <string>
#include <vector>
#include "DepthFirstIterator.h"
#include "Stage.h"
#include "Task.h"
#include "WorkflowMediator.h"
#include "WorkNodeIterator.h"

//a do-absolutely-nothing mediator, only used to check it gets passed down the tree.
namespace {
struct DummyMediator : WorkflowMediator {
    void notify(WorkNode*, const std::string&) override {}
};
}

TEST_CASE("A Stage keeps its children in the order they were added") {
    Stage stage("s1", "Stage");
    stage.add(new Task("t1", "first", "clerk"));
    stage.add(new Task("t2", "second", "clerk"));

    std::vector<WorkNode*> kids = stage.getChildren();
    REQUIRE(kids.size() == 2);
    CHECK(kids[0]->getName() == "first");
    CHECK(kids[1]->getName() == "second");
}

TEST_CASE("A Task is a leaf: it has no children and add() does nothing") {
    Task task("t1", "task", "clerk");
    Task other("t2", "other", "clerk");

    task.add(&other);   // a leaf can't hold anything, so ignored

    CHECK(task.getChildren().empty());
}

TEST_CASE("A Stage can hold another Stage (this is the Composite idea)") {
    Stage outer("outer", "Outer");
    Stage* inner = new Stage("inner", "Inner"); //the outer stage will own
    inner->add(new Task("t1", "deep task", "clerk"));
    outer.add(inner);

    REQUIRE(outer.getChildren().size() == 1);
    CHECK(outer.getChildren()[0]->getName() == "Inner");
    CHECK(outer.getChildren()[0]->getChildren()[0]->getName() == "deep task");
}

TEST_CASE("Stage::add ignores a null pointer, itself, and a duplicate") {
    Stage stage("s1", "Stage");
    Task* task = new Task("t1", "task", "clerk");

    stage.add(nullptr);
    stage.add(&stage);   //a stage inside itself would loop forever
    stage.add(task);
    stage.add(task);     // already added so must not be stored twice

    CHECK(stage.getChildren().size() == 1);
}

TEST_CASE("Stage::remove takes the child out without deleting it") {
    Stage stage("s1", "Stage");
    Task* task = new Task("t1", "task", "clerk");
    stage.add(task);

    stage.remove(task);

    CHECK(stage.getChildren().empty());
    CHECK(task->getName() == "task");   // still allive so delete it ourselves
    delete task;
}

TEST_CASE("Setting the mediator on a Stage reaches every node below it") {
    DummyMediator mediator;
    Stage outer("outer", "Outer");
    Stage* inner = new Stage("inner", "Inner");
    inner->add(new Task("t1", "task", "clerk"));
    outer.add(inner);

    outer.setMediator(&mediator);

    CHECK(outer.getMediator() == &mediator);
    CHECK(inner->getMediator() == &mediator);
    CHECK(inner->getChildren()[0]->getMediator() == &mediator);
}

TEST_CASE("The Iterator visits every node, parents before children") {

    // outer stage
    //  |- inner stage
    //  |    |- a
    //  |    |- b
    //  |- c
    
    Stage outer("outer", "outer");
    Stage* inner = new Stage("inner", "inner");
    inner->add(new Task("a", "a", "clerk"));
    inner->add(new Task("b", "b", "clerk"));
    outer.add(inner);
    outer.add(new Task("c", "c", "clerk"));

    std::vector<std::string> visited;
    std::unique_ptr<WorkNodeIterator> it(outer.createIterator());
    for (it->first(); !it->isDone(); it->next()) {
        visited.push_back(it->current()->getName());
    }

    std::vector<std::string> expected = {"outer", "inner", "a", "b", "c"};
    CHECK(visited == expected);
}
