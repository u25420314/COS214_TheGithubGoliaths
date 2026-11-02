/**
 * @file gui.h
 * @brief Defines the TividyGUI class, which provides the FTXUI-based frontend for the Tividy Workflow System.
 * 
 * This file contains the declaration of the TividyGUI class. It acts as the Presentation layer,
 * interacting with the backend through the TividySystem Facade. It uses the FTXUI library to
 * render a terminal-based user interface, manage state for forms/tabs, and display audit logs.
 */

#ifndef GUI_H
#define GUI_H

#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <string>
#include <vector>
#include <set>
#include "TividySystem.h"

/**
 * @class TividyGUI
 * @brief The Graphical User Interface class for the Tividy system.
 * @author Simon Vogel
 * Encapsulates entire FTXUI layout, event handling, and state management.
 * It provides four main tabs: Dashboard, Instances, Tasks, and Audit Log.
 * It also includes automated testing scripts (University/Insurance demos) 
 * that exercise the backend logic.
 */
class TividyGUI {
private:
    TividySystem& system; ///< Ref to backend Facade, GUI does not own
    ftxui::ScreenInteractive& screen; ///< Reference to FTXUI screen manager. Must outlive this class
    std::vector<std::string> available_definitions; ///< List of workflow names available to start

    std::vector<Participant*> localParticipants; ///< Participants registered by user during this session
    std::vector<std::string> localInstanceIds; ///< Instance IDs created during this session
    std::vector<std::string> audit_lines; ///< Cached lines from backend's audit history for display
    int audit_selected = 0; ///< Currently highlighted line in Audit Log tab
    int selected_instance = 0; ///< Currently highlighted instance in Instances tab

    int active_tab = 0; ///< Index of currently selected tab
    std::vector<std::string> tab_titles = {"Dashboard", "Instances", "Tasks", "Audit Log"}; ///< Titles for sidebar menu
    ftxui::Component tab_menu; ///< Sidebar navigation component
    ftxui::Component main_container; ///< Root FTXUI component containing everything
    ftxui::Component participant_modal; ///< Modal overlay for registering new participants
    bool show_participant_modal = false; ///< Flag to toggle participant registration modal

    std::string p_id, p_name, p_role; ///< Buffers for participant registration form
    int selected_def_idx = 0; ///< Selected index for workflow definition dropdown
    std::string task_id; ///< Buffer for task ID input field
    int selected_participant_idx = 0; ///< Selected index for participant assignment dropdown
    std::vector<std::string> participant_names_dropdown; ///< Formatted names for participant dropdown
    
    std::string message_buffer = ""; ///< Temporary buffer to display success/error messages on underside of the Tasks tab.

    /**
     * @brief Appends custom GUI action to an external text file (here tividy_audit_log.txt)
     * @param msg Action description to log
     */
    void logEvent(const std::string& msg);

    /**
     * @brief Refreshes participant dropdown list based on localParticipants vector
     */
    void updateParticipantDropdown();

    /**
     * @brief Pulls latest history from backend, caches it for Audit Log tab
     * Automatically follows newest line if user is at bottom of list
     */
    void refreshAudit();

    /** @brief Builds Dashboard tab UI components */
    ftxui::Component buildDashboardTab();
    
    /** @brief Builds Instance Manager tab UI components. */
    ftxui::Component buildInstancesTab();
    
    /** @brief Builds Task Command Center tab UI components */
    ftxui::Component buildTasksTab();
    
    /** @brief Builds System Audit Log tab UI components */
    ftxui::Component buildAuditTab();

    /** @brief Runs hardcoded University workflow demo, stopping on completion*/
    void runUniversityDemo();
    
    /** @brief Runs hardcoded Insurance workflow demo, including reject/undo test */
    void runInsuranceDemo();

    /**
     * @brief Parses audit log to find task IDs that are assigned not completed
     * @param instance_id Filter for specific instance ID (empty string for all)
     * @param skip Set task IDs to ignore (tasks that backend has silently completed)
     * @return A vector of pending task IDs
     */
    std::vector<std::string> pendingTasks(const std::string& instance_id, const std::set<std::string>& skip);
    
    /**
     * @brief Checks if a specific workflow instance has state "instance_completed" 
     * @param instance_id The ID of the instance to check
     * @return True if done, otherwise false
     */
    bool finished(const std::string& instance_id);

public:
    /**
     * @brief Constructs the TividyGUI
     * @param sys Reference to TividySystem facade
     * @param scr Reference to FTXUI ScreenInteractive manager
     * @param defs Vector of workflow definition names to display
     */
    TividyGUI(TividySystem& sys, ftxui::ScreenInteractive& scr, const std::vector<std::string>& defs);

    /**
     * @brief Destructor, cleans dynamically allocated participants
     */
    ~TividyGUI();

    /**
     * @brief Starts the main FTXUI event loop. Blocks until user quits.
     */
    void run();
};

#endif