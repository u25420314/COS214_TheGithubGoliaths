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

    // --- GUI Local State ---
    std::vector<WorkflowDefinition*> localDefinitions;
    std::vector<Participant*> localParticipants;
    std::vector<std::string> localInstanceIds;
    std::vector<std::string> auditLog;

    // --- Navigation ---
    int active_tab = 0;
    std::vector<std::string> tab_titles = {"Dashboard", "Builder", "Instances", "Tasks", "Audit Log"};
    ftxui::Component tab_toggle;
    ftxui::Component main_container;
    ftxui::Component participant_modal;
    bool show_participant_modal = false;

    // --- Form Inputs ---
    std::string p_id, p_name, p_role;
    std::string inst_def_name;
    std::string task_id;
    int selected_participant_idx = 0;
    std::vector<std::string> participant_names_dropdown;

    // --- Builder State ---
    std::string b_name, b_stage_name, b_task_name, b_task_role;
    bool b_is_parallel = false;
    struct StageData { std::string name; bool parallel; std::vector<std::pair<std::string, std::string>> tasks; };
    std::vector<StageData> b_stages;

    // --- Helpers ---
    void logEvent(const std::string& msg);
    void updateParticipantDropdown();
    ftxui::Component buildDashboardTab();
    ftxui::Component buildBuilderTab();
    ftxui::Component buildInstancesTab();
    ftxui::Component buildTasksTab();
    ftxui::Component buildAuditTab();

public:
    TividyGUI(TividySystem& sys, ftxui::ScreenInteractive& scr);
    
    // Prevent copying (fixes the FTXUI ScreenInteractive copy error)
    TividyGUI(const TividyGUI&) = delete;
    TividyGUI& operator=(const TividyGUI&) = delete;

    ~TividyGUI();
    void run();
};

#endif