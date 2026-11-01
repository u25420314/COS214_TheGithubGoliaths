#include <vector>
#include <string>
#include <ftxui/component/screen_interactive.hpp>

#include "TividySystem.h"
#include "gui.h"
#include "Scenario.h"

int main() {
    // 1. Build the Facade with the scenario participants
    TividySystem system(scenarioParticipants());

    // 2. Build and register the two workflows
    WorkflowDefinition* university = buildGoliathUniversityWorkflow();
    WorkflowDefinition* insurance  = buildInsuranceClaimWorkflow();

    system.registerDefinition(university);
    system.registerDefinition(insurance);

    // 3. Names the GUI dropdown will show
    std::vector<std::string> available_definitions = {
        university->getName(),
        insurance->getName()
    };

    // 4. Create the interactive screen on the stack (C++17 guaranteed copy elision)
    //    DO NOT use new / heap allocation — ScreenInteractive is non-copyable.
    auto screen = ftxui::ScreenInteractive::Fullscreen();

    // 5. Boot the GUI
    TividyGUI gui(system, screen, available_definitions);
    gui.run();

    return 0;
}