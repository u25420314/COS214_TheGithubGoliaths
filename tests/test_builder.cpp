// BUILDER PATTERN tests.
//Builder creates the Stage/Task tree for us, step by step so nobody has to

#include "doctest.h"
#include <memory>
#include <set>
#include <stdexcept>
#include <string>
#include <vector>
#include "PriorityDecorator.h"
#include "Stage.h"
#include "StandardWorkflowBuilder.h"
#include "Task.h"
#include "WorkNodeIterator.h"
#include "WorkflowDefinition.h"

//returns every node of a finished workflow, in the order the Iterator visits them, so normal.
static std::vector<WorkNode*> allNodes(const WorkflowDefinition& definition) {
    std::vector<WorkNode*> nodes;
    WorkNode* root = const_cast<WorkNode*>(definition.getRoot());
    std::unique_ptr<WorkNodeIterator> it(root->createIterator());
    for (it->first(); !it->isDone(); it->next()) {
        nodes.push_back(it->current());
    }
    return nodes;
}

TEST_CASE("The Builder produces the stages and tasks it was told to") {
    StandardWorkflowBuilder builder;
    builder.setWorkflowName("Demo");
    builder.beginStage("main", false);
    builder.addTask("a", "clerk");
    builder.beginStage("checks", true);   //a stage inside the main stage
    builder.addTask("b", "reviewer");
    builder.endStage();
    builder.endStage();
    WorkflowDefinition definition = builder.getResult();

    CHECK(definition.getName() == "Demo");

    std::vector<WorkNode*> nodes = allNodes(definition);
    REQUIRE(nodes.size() == 4);
    CHECK(nodes[0]->getName() == "main");
    CHECK(nodes[1]->getName() == "a");
    CHECK(nodes[2]->getName() == "checks");
    CHECK(nodes[3]->getName() == "b");
}

TEST_CASE("The Builder remembers the parallel flag and the required role") {
    StandardWorkflowBuilder builder;
    builder.beginStage("main", true);
    builder.addTask("a", "clerk");
    builder.endStage();
    WorkflowDefinition definition = builder.getResult();

    std::vector<WorkNode*> nodes = allNodes(definition);
    Stage* stage = dynamic_cast<Stage*>(nodes[0]);
    REQUIRE(stage != nullptr);
    CHECK(stage->isParallel());
    CHECK(nodes[1]->asTask()->getRequiredRole() == "clerk");
}

TEST_CASE("Every node gets its own unique id") {
    StandardWorkflowBuilder builder;
    builder.beginStage("main", false);
    builder.addTask("a", "clerk");
    builder.addTask("b", "clerk");
    builder.endStage();
    WorkflowDefinition definition = builder.getResult();

    std::set<std::string> ids;   // a set drops duplicates
    std::vector<WorkNode*> nodes = allNodes(definition);
    for (size_t i = 0; i < nodes.size(); i++) {
        ids.insert(nodes[i]->getId());
    }
    CHECK(ids.size() == nodes.size());
}

TEST_CASE("Tasks made by the Builder already have an assignment strategy") {
    StandardWorkflowBuilder builder;
    builder.beginStage("main", false);
    builder.addTask("a", "clerk");
    builder.endStage();
    WorkflowDefinition definition = builder.getResult();

    CHECK(allNodes(definition)[1]->asTask()->getStrategy() != nullptr);
}

TEST_CASE("The Builder refuses calls made in the wrong order") {
    StandardWorkflowBuilder builder;

    CHECK_THROWS_AS(builder.addTask("a", "clerk"), std::logic_error);   //no stage open yet
    CHECK_THROWS_AS(builder.endStage(), std::logic_error);              //nothing to end
    CHECK_THROWS_AS(builder.getResult(), std::logic_error);             //nothing built

    builder.beginStage("main", false);
    CHECK_THROWS_AS(builder.getResult(), std::logic_error);             //stage still open
    builder.endStage();
    CHECK_THROWS_AS(builder.beginStage("second root", false), std::logic_error);   //only one root allowed
}

TEST_CASE("The Builder can be used again after getResult, and the results are independent") {
    StandardWorkflowBuilder builder;

    builder.beginStage("first", false);
    builder.addTask("a", "clerk");
    builder.endStage();
    WorkflowDefinition first = builder.getResult();

    builder.beginStage("second", false);
    builder.addTask("b", "clerk");
    builder.addTask("c", "clerk");
    builder.endStage();
    WorkflowDefinition second = builder.getResult();

    CHECK(allNodes(first).size() == 2);
    CHECK(allNodes(second).size() == 3);
    CHECK(first.getRoot() != second.getRoot());
}

TEST_CASE("withPriority wraps the last task in a PriorityDecorator") {
    StandardWorkflowBuilder builder;
    builder.beginStage("main", false);
    builder.addTask("a", "clerk");
    builder.withPriority(5);
    builder.endStage();
    WorkflowDefinition definition = builder.getResult();

    //stage's only child should now be the decorator, not the bare task.
    WorkNode* root = const_cast<WorkNode*>(definition.getRoot());
    REQUIRE(root->getChildren().size() == 1);
    PriorityDecorator* decorator = dynamic_cast<PriorityDecorator*>(root->getChildren()[0]);
    REQUIRE(decorator != nullptr);
    CHECK(decorator->getLevel() == 5);
}
