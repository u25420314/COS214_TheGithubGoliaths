#ifndef GUI_H
#define GUI_H

#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <string>
#include <vector>
#include "TividySystem.h"

class TividyGUI {
private:
    TividySystem& system;
    ftxui::ScreenInteractive& screen; 
    std::vector<std::string> available_definitions;

    std::vector<Participant*> localParticipants;
    std::vector<std::string> localInstanceIds;
    std::vector<std::string> audit_lines;
    int audit_selected = 0;
    int selected_instance = 0;

    int active_tab = 0;
    std::vector<std::string> tab_titles = {"Dashboard", "Instances", "Tasks", "Audit Log"};
    ftxui::Component tab_menu;
    ftxui::Component main_container;
    ftxui::Component participant_modal;
    bool show_participant_modal = false;

    std::string p_id, p_name, p_role;
    int selected_def_idx = 0;
    std::string task_id;
    int selected_participant_idx = 0;
    std::vector<std::string> participant_names_dropdown;
    
    std::string message_buffer = ""; // <-- NEW: Shows errors on screen

    void logEvent(const std::string& msg);
    void updateParticipantDropdown();
    void refreshAudit();
    ftxui::Component buildDashboardTab();
    ftxui::Component buildInstancesTab();
    ftxui::Component buildTasksTab();
    ftxui::Component buildAuditTab();

    void runUniversityDemo();
    void runInsuranceDemo();
    std::vector<std::string> pendingTasks();
    bool finished();

public:
    TividyGUI(TividySystem& sys, ftxui::ScreenInteractive& scr, const std::vector<std::string>& defs);
    ~TividyGUI();
    void run();
};

#endif