#include "doctest.h"

#include "Command.h"
#include "CommandHistory.h"

#include <string>
#include <vector>

namespace {
    //simple command to use whithout having to test command itself
    struct CountingCommand : Command {
        int& value;
        
        CountingCommand(int& value) : Command(nullptr), value(value) {}
        void execute() override { value++; }
        void undo() override { value--; }
    };

    struct StringCommand : Command {
        std::vector<std::string>& log;
        std::string value;

        StringCommand(std::vector<std::string>& log, std::string value) : Command(nullptr), log(log), value(value) {}
        void execute() override { log.push_back("execute " + value); }
        void undo() override { log.push_back("undo " + value); }
    };
}

TEST_CASE("CommandHistory executes an added command") {
    CommandHistory test;
    int count = 0;

    test.executeCommand(new CountingCommand(count));
    CHECK(count == 1);
}

TEST_CASE("CommandHistory executes more than one added command") {
    CommandHistory test;
    int count = 0;

    test.executeCommand(new CountingCommand(count));
    test.executeCommand(new CountingCommand(count));
    test.executeCommand(new CountingCommand(count));
    test.executeCommand(new CountingCommand(count));
    
    CHECK(count == 4);
}

TEST_CASE("Undoes the most recent command") {
    CommandHistory test;
    int count = 0;

    test.executeCommand(new CountingCommand(count));
    CHECK(count == 1);

    test.undo();
    CHECK(count == 0);
}

TEST_CASE("Undoes commands in reverse order") {
    CommandHistory test;
    std::vector<std::string> log;

    test.executeCommand(new StringCommand(log, "A"));
    test.executeCommand(new StringCommand(log, "B"));

    test.undo();
    test.undo();

    CHECK(log[2] == "undo B");
    CHECK(log[3] == "undo A");
}

TEST_CASE("Undoes nothing when history is empty") {
    CommandHistory test;
    int count = 0;

    CHECK_NOTHROW(test.undo());
}

TEST_CASE("Executed nothing when a command is null") {
    CommandHistory test;
    
    CHECK_NOTHROW(test.executeCommand(nullptr));
    CHECK_NOTHROW(test.undo());
}