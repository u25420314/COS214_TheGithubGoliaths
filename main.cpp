//EXTREMELY basic just to get started on the UI
#include "TividySystem.h"
#include "gui.h"

int main() {
    // 1. Create the system (your backend)
    TividySystem system;

    // 2. Create the GUI, passing the system to it
    TividyGUI gui(system);

    // 3. Start the GUI loop
    gui.run();

    return 0;
}