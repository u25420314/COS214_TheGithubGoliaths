#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "RoleBasedAssignment.h"
#include "Participant.h"
#include "Task.h"

#include <vector>
#include <string>

TEST_CASE("RoleBasedAssignments selects correct participant"){
    std::vector<std::string> studentRole;
    studentRole.push_back("Student");

    std::vector<std::string> lecturerRole;
    lecturerRole.push_back("Lecturer");

    std::vector<std::string> adminRole;
    lecturerRole.push_back("Admin");

    Participant john("P01", "John", studentRole);
    Participant kane("P02", "kane", lecturerRole);
    Participant sarah("P03", "Sarah", adminRole);

    std::vector<Participant*> participants;
    participants.push_back(&john);
    participants.push_back(&kane);
    participants.push_back(&sarah);

    Task task("T01", "Mark Assignment", "Lecturer");

    RoleBasedAssignment strategy;

    Participant* selected = strategy.choose(task, participants);

    CHECK(selected == &kane);

}