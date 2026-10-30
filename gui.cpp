#include "gui.h"

using namespace ftxui;

TividyGUI::TividyGUI(TividySystem& system) {
    (void)system;  // TODO
}

TividyGUI::~TividyGUI() {}

// ---- rendering ----

Element TividyGUI::render() { 
    return vbox({
        text("TIVIDY GUI SYSTEM") | bold | center,
        separator(),
        text("Press 'q' or 'Esc' to quit.") | center,
        filler()
    }) | border;
}

Element TividyGUI::renderIndustrySelect() { return text("TODO"); }
Element TividyGUI::renderStartInstance() { return text("TODO"); }
Element TividyGUI::renderWorkList() { return text("TODO"); }
Element TividyGUI::renderTaskActions() { return text("TODO"); }
Element TividyGUI::renderProgress() { return text("TODO"); }
Element TividyGUI::renderHistory() { return text("TODO"); }

// ---- input ----

bool TividyGUI::handleInput(Event event) { (void)event; return false; }
bool TividyGUI::handleIndustryInput(Event event) { (void)event; return false; }
bool TividyGUI::handleStartInstanceInput(Event event) { (void)event; return false; }
bool TividyGUI::handleWorkListInput(Event event) { (void)event; return false; }
bool TividyGUI::handleTaskActionsInput(Event event) { (void)event; return false; }
bool TividyGUI::handleProgressInput(Event event) { (void)event; return false; }
bool TividyGUI::handleHistoryInput(Event event) { (void)event; return false; }

// ---- actions ----

void TividyGUI::startSelectedWorkflow() {}
void TividyGUI::assignSelectedTask() {}
void TividyGUI::completeSelectedTask() {}
void TividyGUI::approveSelectedTask() {}
void TividyGUI::rejectSelectedTask() {}
void TividyGUI::undoLastAction() {}

// ---- data loading ----

void TividyGUI::loadAvailableTasks() {}
void TividyGUI::loadHistory() {}

void TividyGUI::run() {
    // Create the interactive screen
    auto screen = ScreenInteractive::TerminalOutput();

    // Create a basic component that uses your render() function
    auto renderer = Renderer([this] {
        return render(); 
    });

    // Add a way to quit (capture screen by reference!)
    auto component = CatchEvent(renderer, [&screen](Event event) {
        if (event == Event::Character('q') || event == Event::Escape) {
            screen.ExitLoopClosure()();
            return true;
        }
        return false;
    });

    // Start the loop
    screen.Loop(component);
}