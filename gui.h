#pragma once

#include <memory>
#include <string>
#include <vector>

#include "ftxui/component/component.hpp"
#include "ftxui/component/captured_mouse.hpp"
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>

#include "TividySystem.h"

// NOTE: no "using namespace ftxui;" in a header. Put it in gui.cpp instead.

class TividyGUI {
public:
    explicit TividyGUI(TividySystem& system);
    ~TividyGUI();

    void run();

private:
    // TODO: state (current view, selected indices, message buffer, ...)
    // TODO: views (industry select, work list, task actions, progress, history, ...)
 
    // ---- rendering: one per view ----
    ftxui::Element render();
    ftxui::Element renderIndustrySelect();
    ftxui::Element renderStartInstance();
    ftxui::Element renderWorkList();
    ftxui::Element renderTaskActions();
    ftxui::Element renderProgress();
    ftxui::Element renderHistory();

    // ---- input: one per view ----
    bool handleInput(ftxui::Event event);
    bool handleIndustryInput(ftxui::Event event);
    bool handleStartInstanceInput(ftxui::Event event);
    bool handleWorkListInput(ftxui::Event event);
    bool handleTaskActionsInput(ftxui::Event event);
    bool handleProgressInput(ftxui::Event event);
    bool handleHistoryInput(ftxui::Event event);

    // ---- actions: these are the only functions that talk to TividySystem ----
    void startSelectedWorkflow();
    void assignSelectedTask();
    void completeSelectedTask();   // needs a facade method that doesn't exist yet
    void approveSelectedTask();    // needs a facade method that doesn't exist yet
    void rejectSelectedTask();     // needs a facade method that doesn't exist yet
    void undoLastAction();

    // ---- data loading ----
    void loadAvailableTasks();     // needs a facade method that doesn't exist yet
    void loadHistory();            // needs a facade method that doesn't exist yet
};