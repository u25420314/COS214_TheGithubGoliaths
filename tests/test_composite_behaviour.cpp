// COMPOSITE PATTERN - behaviour tests.
// whether tree behaves correctly->stage is finished and tasks run 

#include "doctest.h"
#include <memory>
#include <vector>
#include "CompletedState.h"
#include "Stage.h"
#include "Task.h"

//pretend a task is done and set its state directly so these tests
// only depend on Stage and Task, not on the State classes.
static void finish(WorkNode* node) {
    node->asTask()->setState(new CompletedState());
}

TEST_CASE("A Stage is complete only when ALL of its children are complete") {
    Stage stage("s1", "Stage", true);
    stage.add(new Task("t1", "a", "clerk"));
    stage.add(new Task("t2", "b", "clerk"));

    CHECK_FALSE(stage.isComplete());
    finish(stage.getChildren()[0]);
    CHECK_FALSE(stage.isComplete());   //one done, one still open
    finish(stage.getChildren()[1]);
    CHECK(stage.isComplete());
}

TEST_CASE("A Stage inside a Stage must also be complete") {
    Stage outer("outer", "Outer");
    Stage* inner = new Stage("inner", "Inner", true);
    inner->add(new Task("t1", "a", "clerk"));
    outer.add(inner);
    outer.add(new Task("t2", "b", "clerk"));

    finish(inner->getChildren()[0]);
    CHECK(inner->isComplete());
    CHECK_FALSE(outer.isComplete());//task b is not done yet

    finish(outer.getChildren()[1]);
    CHECK(outer.isComplete());
}

TEST_CASE("A sequential Stage offers only its first unfinished child") {
    //Sequential means: one after another.
    Stage stage("s1", "Stage", false);
    stage.add(new Task("t1", "a", "clerk"));
    stage.add(new Task("t2", "b", "clerk"));
    stage.add(new Task("t3", "c", "clerk"));

    REQUIRE(stage.getReadyChildren().size() == 1);
    CHECK(stage.getReadyChildren()[0]->getName() == "a");

    finish(stage.getChildren()[0]);
    REQUIRE(stage.getReadyChildren().size() == 1);
    CHECK(stage.getReadyChildren()[0]->getName() == "b");//a is done, so b is next
}

TEST_CASE("A parallel Stage offers all of its unfinished children") {
    // Parallel means: all at the same time.
    Stage stage("s1", "Stage", true);
    stage.add(new Task("t1", "a", "clerk"));
    stage.add(new Task("t2", "b", "clerk"));
    stage.add(new Task("t3", "c", "clerk"));
    CHECK(stage.getReadyChildren().size() == 3);

    finish(stage.getChildren()[1]);
    CHECK(stage.getReadyChildren().size() == 2);// b is done, a and c remain
}

TEST_CASE("collectReady gathers the tasks that may run now, through nested stages") {
    
    Stage outer("outer", "Outer", false);
    Stage* parallelStage = new Stage("p", "Parallel", true);
    parallelStage->add(new Task("t1", "a", "clerk"));
    parallelStage->add(new Task("t2", "b", "clerk"));
    outer.add(parallelStage);
    outer.add(new Task("t3", "c", "clerk"));

    std::vector<Task*> ready;
    outer.collectReady(ready);
    REQUIRE(ready.size() == 2);
    CHECK(ready[0]->getName() == "a");
    CHECK(ready[1]->getName() == "b");

    finish(parallelStage->getChildren()[0]);
    finish(parallelStage->getChildren()[1]);

    ready.clear();
    outer.collectReady(ready);
    REQUIRE(ready.size() == 1);
    CHECK(ready[0]->getName() == "c");
}

TEST_CASE("clone() copies the whole tree, so the copy is independent") {
    Stage original("s1", "Stage");
    original.add(new Task("t1", "a", "clerk"));
    std::unique_ptr<WorkNode> copy(original.clone());

    REQUIRE(copy->getChildren().size() == 1);
    CHECK(copy->getChildren()[0]->getName() == "a");
    CHECK(copy->getChildren()[0] != original.getChildren()[0]);
    //deep copy

    // Finishing the copy's task must not touch the original's task.
    finish(copy->getChildren()[0]);
    CHECK(copy->isComplete());
    CHECK_FALSE(original.isComplete());
}

TEST_CASE("clone() gives a Task a fresh state, even if the original had progressed") {
    Task original("t1", "a", "clerk");
    original.setState(new CompletedState());

    std::unique_ptr<WorkNode> copy(original.clone());

    CHECK(original.isComplete());
    CHECK_FALSE(copy->isComplete());   //each new workflow instance starts from scratch
}
