#include "doctest.h"
#include <memory>
#include <string>
#include <vector>
#include "Task.h"
#include "Participant.h"
#include "TaskState.h"

namespace{
    typedef std::unique_ptr<Task> TaskPtr;
    typedef std::unique_ptr<Participant> PersonPtr;

    TaskPtr newTask() {
        return TaskPtr(new Task("t1", "Assess Damage", "Accessor"));

    }

    PersonPtr newPerson(const std::string& id){
        return PersonPtr(new Participant(id, "Person " + id, std::vector<std::string>(1, "Assessor")));

    }

    std::string stateOf(Task& t) {
        return t.getState()->getName();

    }

    bool isTerminal(Task& t) {
        return t.getState()->isTerminal();
    }


    enum Action { ASSIGN, START, COMPLETE, REJECT, ESCALATE, CANCEL, ACTION_COUNT};
    enum StateId { AVAILABLE, ASSIGNED, IN_PROGRESS, COMPLETED, REJECTED, ESCALATED, CANCELLED, STATE_COUNT};

    const char* const kActionNames[ACTION_COUNT]= { "assign ", "start", "complete", "reject", "escalate", "cancel"};

    const char* const kStateNames[STATE_COUNT] = {"Available", "Assigned", "InProgress", "Completed", "Rejected", "Escalated", "Cancelled"};


    //perfrom one action on the tast

    void apply(Task& t, Participant* p, Action a){
        switch (a)
        {
        case ASSIGN:
            t.assign(p);
            break;
        case START:
            t.start();
            break;
        case COMPLETE:
            t.complete();
            break;
        case REJECT:
            t.reject();
            break;
        case ESCALATE:
            t.escalate();
            break;
        case CANCEL:
            t.cancel();
            break;
        default:
            break;
        
        
        }
    }

    void goTo(Task& t, Participant* p, StateId s){
        switch (s){
            case AVAILABLE:
                break;
            case ASSIGNED:
                t.assign(p);
                break;
            case IN_PROGRESS:
                t.assign(p);
                t.start();
                break;
            case COMPLETED:
                t.assign(p);
                t.start();
                t.complete();

                break;
            case REJECTED:
                t.assign(p);
                t.start();
                t.reject();
                break;
            
            case ESCALATED:
                t.assign(p);
                t.start();
                t.escalate();
                break;

            case CANCELLED:
                t.cancel();
                break;

            default:
                break;
        }
    }

    //row is current state, colum is action, call is the resulting state

    const char* const R = "REFUSED";
    const char* const kExpected[STATE_COUNT][ACTION_COUNT] = {     
        { "Assigned", R, R,R,R,"Cancelled" },
        { R, "InProgress", R,R,R,"Cancelled" },
        { R,R,"Completed",  "Rejected",   "Escalated",  "Cancelled" },
        { R,R,R,R,R,R},
        { "Assigned", R,R,R,R,R},
        { "Assigned", R,R,R,R,R},
        { R,R,R,R,R,R}
    };
}

//1. whole transtion table 7 states and 6 actions, should be 42 checks

TEST_CASE("every state x action combination matches the transition table"){
    for(int i = 0; i < STATE_COUNT; i++){
        for (int j = 0; j < ACTION_COUNT; j++)
        {
            INFO("state" << std::string(kStateNames[i]) << " action=" << std::string(kActionNames[j]));

            TaskPtr t = newTask();
            PersonPtr p = newPerson("p1");
            goTo(*t, p.get(), static_cast<StateId>(i));
            REQUIRE_EQ(stateOf(*t), std::string(kStateNames[i])); //setup has no reached state


            bool refused = false;
            try
            {
                apply(*t, p.get(), static_cast<Action>(j));
            }
            catch(const InvalidTransitionError)
            {
                refused = true;
            }

            std::string expected = kExpected[i][j];
            if(expected == R){
                CHECK(refused);
                CHECK_EQ(stateOf(*t), std::string(kStateNames[i]));
            }else{
                CHECK_FALSE(refused);
                CHECK_EQ(stateOf(*t), expected);
            }
            
        }
        
    }
}

//side effects and journeys
TEST_CASE("a new task starts Avaliable with no assignee"){
    TaskPtr t = newTask();
    CHECK_EQ(stateOf(*t), "Available");
    CHECK(t->getAssignee() == nullptr);


}

TEST_CASE("assign records who the assignee is"){
    TaskPtr t = newTask();
    PersonPtr alice =  newPerson("alice");
    t->assign(alice.get());
    CHECK(t->getAssignee() == alice.get());

}

TEST_CASE("a refused assgn leaves the original assignee untouched"){
    TaskPtr t = newTask();
    PersonPtr alice = newPerson("alice");
    PersonPtr bob = newPerson("bob");
    t->assign(alice.get());
    CHECK_THROWS_AS(t->assign(bob.get()), InvalidTransitionError);//means already assigned
    CHECK(t->getAssignee() == alice.get());
}

TEST_CASE("rework loop: after a rejecrion the task is reassigned and can be finished"){
    TaskPtr t = newTask();
    PersonPtr alice = newPerson("alice");
    PersonPtr bob = newPerson("bob");
    goTo(*t, alice.get(), REJECTED);
    t->assign(bob.get());
    CHECK(t->getAssignee() == bob.get());
    t->start();
    t->complete();
    CHECK_EQ(stateOf(*t), "Completed");
}

TEST_CASE("esclation loop - an escaalted task goes to a higher athority and finishes"){
    TaskPtr t = newTask();
    PersonPtr junior = newPerson("senior");
    PersonPtr senior = newPerson("senior");
    goTo(*t, junior.get(), ESCALATED);
    t->assign(senior.get());
    CHECK(t->getAssignee() == senior.get());
    t->start();
    t->complete();
    CHECK_EQ(stateOf(*t), "Completed");

}

TEST_CASE("a task can be rejected and reworked more than once"){
    TaskPtr t = newTask();
    PersonPtr p = newPerson("p1");
    for (int i = 0; i < 3; i++)
    {
        t->assign(p.get());
        t->start();
        t->reject();
        CHECK_EQ(stateOf(*t), "Rejected");
    }

    t->assign(p.get());
    t->start();
    t->complete();
    CHECK_EQ(stateOf(*t), "Completed");
    
}


//3 terminal statses and the helpers

TEST_CASE("only completed and cancelled are terminal"){
    for(int i = 0; i <STATE_COUNT; i++){
        INFO("state = "<< std::string(kStateNames[i]));
        TaskPtr t = newTask();
        PersonPtr p  = newPerson("p1");
        goTo(*t,  p.get(), static_cast<StateId>(i));
        CHECK_EQ(isTerminal(*t), (i == COMPLETED || i == CANCELLED));
    }
}

TEST_CASE("task::isCOmplete follows the state"){
    TaskPtr t = newTask();
    PersonPtr p = newPerson("p1");
    CHECK_FALSE(t->isComplete());
    goTo(*t, p.get(), COMPLETED);
    CHECK(t->isComplete());

    
}

TEST_CASE("clone gives an independant state of the same kind"){
    for(int i = 0; i < STATE_COUNT; i++){
        INFO("State=" << std::string(kStateNames[i]));
        TaskPtr t = newTask();
        PersonPtr p = newPerson("p1");
        goTo(*t, p.get(), static_cast<StateId>(i));

        std::unique_ptr<TaskState> copy(t->getState()->clone());
        CHECK_EQ(copy->getName(), std::string(kStateNames[i]));
        CHECK_EQ(copy->isTerminal(), t->getState()->isTerminal());
        CHECK_EQ(copy->isComplete(), t->getState()->isComplete());
        CHECK(copy.get() != t->getState()); //gets new object not same
    }
}


//bad input and error reporting

TEST_CASE("assigning a null participant is refusd and nothing changes"){
    TaskPtr t = newTask();
    CHECK_THROWS_AS(t->assign(nullptr), std::invalid_argument);
    CHECK_EQ(stateOf(*t), "Available");
    CHECK(t->getAssignee() == nullptr);
    
}


TEST_CASE("reassgining a rejected task to nobody is refused and it will stay rejected"){
    TaskPtr t = newTask();
    PersonPtr p = newPerson("p1");
    goTo(*t, p.get(), REJECTED);
    CHECK_THROWS_AS(t->assign(nullptr), std::invalid_argument);
    CHECK_EQ(stateOf(*t), "Rejected");
}

TEST_CASE("the error message names the failed action and the state"){
    TaskPtr t = newTask();
    try
    {
        t->complete();
        CHECK(false); //must never get here
    }
    catch(const std::logic_error& e)//tui should be avle to cach logic error 
    {
        std::string msg = e.what();
        CHECK(msg.find("complete") != std::string::npos);
        CHECK(msg.find("Available") != std::string::npos);
    }
    
}


//isolation

TEST_CASE("two tasks keep independent states "){
    TaskPtr a = newTask();
    TaskPtr b = newTask();
    PersonPtr p = newPerson("p1");
    goTo(*a, p.get(), IN_PROGRESS);
    CHECK_EQ(stateOf(*a), "InProgress");
    CHECK_EQ(stateOf(*b), "Available");
}

TEST_CASE("only the Completed state reports isComplete"){
    for (int i = 0; i < STATE_COUNT; i++)
    {
        INFO("state= " << std::string(kStateNames[i]));
        TaskPtr t = newTask();
        PersonPtr p = newPerson("p1");
        goTo(*t, p.get(), static_cast<StateId>(i));
        CHECK_EQ(t->getState()->isComplete(), (i == COMPLETED));
    }
    
}
