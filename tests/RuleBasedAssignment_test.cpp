#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "RuleBasedAssignment.h"
#include "Participant.h"
#include "Task.h"

#include <string>
#include <vector>

TEST_CASE("RuleBasedAssignment selects participant with required role"){
    std::vector<std::string> lecturerRoles;
    lecturerRoles.push_back("Lecturer");

    std::vector<std::string> studentRoles;
    studentRoles.push_back("Student");

    Participant john("P01", "John", studentRoles);
    Participant kane("P02", "Kane", lecturerRoles);

    std::vector<Participant*> participants;
    participants.push_back(&john);
    participants.push_back(&kane);

    Task task("T01", "Mark Assignment", "Lecturer");

    RuleBasedAssignment strategy("role", "Lecturer");

    Participant* selected = strategy.choose(task, participants);

    CHECK(selected == &kane);


}

TEST_CASE("RuleBasedAssignment selects participant under max workload"){
    std::vector<std::string> lecturerRoles;
    lecturerRoles.push_back("Lecturer");

    Participant john("P01", "John", lecturerRoles);
    Participant kane("P02", "Kane", lecturerRoles);

    john.incrementWorkload();
    john.incrementWorkload();
    john.incrementWorkload();

    kane.incrementWorkload();

    std::vector<Participant*> participants;
    participants.push_back(&john);
    participants.push_back(&kane);

    Task task("T03", "Mark Assignments", "Lecturer");
    RuleBasedAssignment strategy("maxWorkload", "2");
    Participant* selected = strategy.choose(task, participants);

    CHECK(selected == &kane);

}