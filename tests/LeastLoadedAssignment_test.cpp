#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "LeastLoadedAssignment.h
#include "Participant.h"
#include "Task.h"

#include <vector>
#include <string>

TEST_CASE("LeastLoadedAssignments selects correct participant"){
    std::vector<std::string> studentRole;
    studentRole.push_back("Student");

    std::vector<std::string> lecturerRole;
    lecturerRole.push_back("Lecturer");

    std::vector<std::string> adminRole;
    lecturerRole.push_back("Admin");

    Participant john("P01", "John", studentRole);
    Participant kane("P02", "kane", studentRole);
    Participant sarah("P03", "Sarah", adminRole);

    std::vector<Participant*> participants;
    participants.push_back(&john);
    participants.push_back(&kane);
    participants.push_back(&sarah);

    john.incrementWorkload();
    kane.incrementWorkload();
    kane.incrementWorkload();
    kane.incrementWorkload();
    john.incrementWorkload();
    john.incrementWorkload();
    john.incrementWorkload();
    john.incrementWorkload();

    Task task("T01", "Mark Assignment", "Student");

    LeastLoadedAssignment strategy;

    Participant* selected = strategy.choose(task, participants);

    CHECK(selected == &kane);

}