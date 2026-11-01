#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "Participant.h"
#include <string>
#include <vector>

TEST_CASE("Participant tests"){
    std::vector<std::string> roles;
    roles.push_back("Lecturer");
    roles.push_back("Admin");

    Participant p("P01", "Kane", roles);
    CHECK(p.getId() == "P01");
    CHECK(p.getName() == "Kane");
    CHECK(p.getWorkload() == 0);

    CHECK(p.getRoles().size() == 2);
    CHECK(p.getRoles()[0] == "Lecturer");
    CHECK(p.getRoles()[1] == "Admin");

    CHECK(p.hasRole("Lecturer") == true);
    CHECK(p.hasRole("team") == false);
    p.incrementWorkload();

    CHECK(p.getWorkload() == 1);
    p.decrementWorkload();
    CHECK(p.getWorkload() == 0);

}