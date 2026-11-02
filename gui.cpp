#include "gui.h"
#include <algorithm>
#include <functional>
#include <iostream>
#include <set>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <ctime>

using namespace ftxui;

std::string getTimestamp() {
    std::time_t now = std::time(nullptr);
    std::tm* localTime = std::localtime(&now);
    std::ostringstream oss;
    oss << std::put_time(localTime, "[%Y-%m-%d %H:%M:%S]");
    return oss.str();
}

namespace {

class FallbackKeys : public ComponentBase {
public:
    FallbackKeys(Component child, std::function<bool(Event)> fallback)
        : fallback_(std::move(fallback)) { Add(std::move(child)); }
    bool OnEvent(Event event) override {
        return ComponentBase::OnEvent(event) || fallback_(event);
    }
private:
    std::function<bool(Event)> fallback_;
};

class ScrollableList : public ComponentBase {
public:
    ScrollableList(std::vector<std::string>* items, int* selected, int max_visible)
        : items_(items), selected_(selected), max_visible_(max_visible), offset_(0) {}

    bool OnEvent(Event event) override {
        if (items_->empty()) return false;
        if (*selected_ < 0) *selected_ = 0;
        if (*selected_ >= (int)items_->size()) *selected_ = (int)items_->size() - 1;

        if (event == Event::ArrowUp) { if (*selected_ > 0) (*selected_)--; return true; }
        if (event == Event::ArrowDown) { if (*selected_ < (int)items_->size() - 1) (*selected_)++; return true; }
        if (event == Event::PageUp) { *selected_ = std::max(0, *selected_ - max_visible_); return true; }
        if (event == Event::PageDown) { *selected_ = std::min((int)items_->size() - 1, *selected_ + max_visible_); return true; }
        return false;
    }

    Element Render() override {
        if (items_->empty()) return text("None") | dim;
        int totalItems = (int)items_->size();
        
        if (*selected_ < offset_) offset_ = *selected_;
        else if (*selected_ >= offset_ + max_visible_) offset_ = *selected_ - max_visible_ + 1;
        offset_ = std::max(0, std::min(offset_, std::max(0, totalItems - max_visible_)));

        Elements els;
        if (offset_ > 0) els.push_back(text("▲ More above ▲") | center | color(Color::Yellow) | dim);
        for (int i = offset_; i < std::min(totalItems, offset_ + max_visible_); ++i) {
            Element e = text(" " + (*items_)[i]);
            if (i == *selected_) e = e | inverted;
            els.push_back(e);
        }
        if (offset_ + max_visible_ < totalItems) els.push_back(text("▼ More below ▼") | center | color(Color::Yellow) | dim);
        return vbox(std::move(els));
    }

    bool Focusable() const override { return true; }
private:
    std::vector<std::string>* items_;
    int* selected_;
    int max_visible_;
    int offset_;
};

}  

TividyGUI::TividyGUI(TividySystem& sys, ftxui::ScreenInteractive& scr, const std::vector<std::string>& defs) 
    : system(sys), screen(scr), available_definitions(defs) {
    
    MenuOption sidebar_option = MenuOption::Vertical();
    sidebar_option.entries_option.transform = [](const EntryState& s) {
        Element e = text((s.active ? "▶ " : "  ") + s.label);
        if (s.active) e = e | bold | color(Color::Cyan);
        if (s.focused) e = e | inverted;
        return e;
    };
    tab_menu = Menu(&tab_titles, &active_tab, sidebar_option);

    if (available_definitions.empty()) available_definitions.push_back("(No definitions loaded)");
    participant_names_dropdown.push_back("(Register a participant first)");

    auto in_p_id = Input(&p_id, "ID (e.g. p1)");
    auto in_p_name = Input(&p_name, "Name (e.g. Alice)");
    auto in_p_role = Input(&p_role, "Role (e.g. clerk)");
    
    auto btn_add_participant = Button("Register", [&] {
        if (p_id.empty() || p_name.empty()) return;
        std::vector<std::string> roles;
        if (!p_role.empty()) roles.push_back(p_role);
        Participant* p = new Participant(p_id, p_name, roles);
        localParticipants.push_back(p);
        updateParticipantDropdown();
        logEvent("Registered Participant: " + p_name + " (Role: " + p_role + ")");
        p_id = ""; p_name = ""; p_role = "";
        show_participant_modal = false;
    });

    auto modal_buttons = Container::Horizontal(std::vector<Component>{
        Button("Cancel", [&] { show_participant_modal = false; }) | border,
        btn_add_participant | border
    });
    auto modal_form = Container::Vertical(std::vector<Component>{ in_p_id, in_p_name, in_p_role, modal_buttons });
    participant_modal = Renderer(modal_form, [in_p_id, in_p_name, in_p_role, modal_buttons] {
        return vbox({
            text("Register New Participant") | bold | hcenter | color(Color::Cyan),
            separator(),
            in_p_id->Render(),
            in_p_name->Render(),
            in_p_role->Render(),
            separator(),
            modal_buttons->Render(),
            text("Esc to cancel") | dim | hcenter
        }) | border | size(WIDTH, EQUAL, 50);
    });

    std::vector<Component> tabs = {
        buildDashboardTab(), buildInstancesTab(), buildTasksTab(), buildAuditTab()
    };
    auto tab_container = Container::Tab(tabs, &active_tab);
    auto layout = Container::Horizontal(std::vector<Component>{ tab_menu, tab_container });

    auto main_ui = Renderer(layout, [this, tab_container] {
        bool side_focused = !show_participant_modal && tab_menu->Focused();
        bool main_focused = !show_participant_modal && !side_focused;
        Color side_color = side_focused ? Color::Cyan : Color::GrayDark;
        Color main_color = main_focused ? Color::Cyan : Color::GrayDark;

        return hbox(std::vector<Element>{
            vbox({
                text(" TIVIDY ") | bold | hcenter | color(Color::Cyan),
                separator(),
                tab_menu->Render() | flex,
                separator(),
                text(" ↑/↓  switch tab") | dim,
                text(" →    open tab") | dim,
                text(" Esc  back to menu") | dim,
                text(" Q    quit (menu)") | dim
            }) | borderStyled(ROUNDED, side_color) | size(WIDTH, EQUAL, 25),
            tab_container->Render() | flex | borderStyled(ROUNDED, main_color) | flex
        });
    });

    main_container = Make<FallbackKeys>(
        Modal(main_ui, participant_modal, &show_participant_modal),
        [this](Event event) {
            if (event == Event::Escape) {
                if (show_participant_modal) { show_participant_modal = false; return true; }
                if (!tab_menu->Focused()) { tab_menu->TakeFocus(); return true; }
                return false;
            }
            if ((event == Event::Character('q') || event == Event::Character('Q')) &&
                !show_participant_modal && tab_menu->Focused()) {
                screen.ExitLoopClosure()();
                return true;
            }
            return false;
        });
}

TividyGUI::~TividyGUI() { for (auto p : localParticipants) delete p; }
void TividyGUI::run() { screen.Loop(main_container); }

void TividyGUI::logEvent(const std::string& action) {
    std::ofstream log("tividy_audit_log.txt", std::ios::app);
    if (log.is_open()) {
        log << getTimestamp() << " [GUI ACTION] " << action << "\n";
        log.close();
    }
}

void TividyGUI::updateParticipantDropdown() {
    participant_names_dropdown.clear();
    for (auto p : localParticipants) participant_names_dropdown.push_back(p->getName() + " (" + p->getId() + ")");
    if (participant_names_dropdown.empty()) participant_names_dropdown.push_back("(Register a participant first)");
    if (selected_participant_idx >= (int)participant_names_dropdown.size()) selected_participant_idx = 0;
}

void TividyGUI::refreshAudit() {
    std::vector<std::string> fresh;
    try {
        std::vector<std::string> history = system.getHistory();
        size_t start = history.size() > 500 ? history.size() - 500 : 0;
        for (size_t i = start; i < history.size(); ++i)
            fresh.push_back("[" + std::to_string(i + 1) + "] " + history[i]);
    } catch (...) { fresh.push_back("Error reading history."); }
    bool following = audit_lines.empty() || audit_selected >= (int)audit_lines.size() - 1;
    audit_lines.swap(fresh);
    if (audit_lines.empty()) audit_selected = 0;
    else if (following) audit_selected = (int)audit_lines.size() - 1;
    else audit_selected = std::min(audit_selected, (int)audit_lines.size() - 1);
}

Component TividyGUI::buildDashboardTab() {
    auto btn_run_uni = Button("Run University Demo Script", [&] { runUniversityDemo(); });
    auto btn_run_ins = Button("Run Insurance Demo Script", [&] { runInsuranceDemo(); });
    auto btn_reg_participant = Button("Register New Participant", [&] { show_participant_modal = true; });

    auto controls = Container::Vertical(std::vector<Component>{ btn_run_uni, btn_run_ins, btn_reg_participant });

    return Renderer(controls, [this, btn_run_uni, btn_run_ins, btn_reg_participant] {
        return vbox({
            text("System Overview") | bold | color(Color::Green),
            separator(),
            vbox({
                text("Available Definitions : " + std::to_string(available_definitions.size())),
                text("Active Instances      : " + std::to_string(localInstanceIds.size())),
                text("Registered Participants: " + std::to_string(localParticipants.size())),
                text("Audit Log File        : tividy_audit_log.txt"),
            }) | border,
            separator(),
            text("Automated Backend Testing:") | bold,
            btn_run_uni->Render(),
            btn_run_ins->Render(),
            separator(),
            btn_reg_participant->Render()
        }) | flex;
    });
}

Component TividyGUI::buildInstancesTab() {
    auto dropdown = Dropdown(&available_definitions, &selected_def_idx);
    auto btn_start = Button("Start Workflow", [&] {
        if (available_definitions.empty() || selected_def_idx >= (int)available_definitions.size()) return;
        if (available_definitions[selected_def_idx] == "(No definitions loaded)") return;
        try {
            WorkflowData data; 
            std::string id = system.startWorkflow(available_definitions[selected_def_idx], data);
            if (id != "Definition not found") {
                localInstanceIds.push_back(id);
                selected_instance = (int)localInstanceIds.size() - 1;
                logEvent("Started Workflow: " + available_definitions[selected_def_idx] + " (ID: " + id + ")");
            }
        } catch (const std::exception& e) { logEvent(std::string("ERROR: ") + e.what()); }
    });

    auto list = Make<ScrollableList>(&localInstanceIds, &selected_instance, 8);
    auto controls = Container::Vertical(std::vector<Component>{ dropdown, btn_start, list });

    return Renderer(controls, [this, dropdown, btn_start, list] {
        return vbox({
            text("Instance Manager") | bold | color(Color::Green),
            separator(),
            text("Select Definition to Start:"),
            dropdown->Render(),
            btn_start->Render(),
            separator(),
            vbox({ text("Active Instances:") | underlined, list->Render() }) | border
        }) | flex;
    });
}

Component TividyGUI::buildTasksTab() {
    auto in_task = Input(&task_id, "Task ID (e.g. n2, n7)");
    auto dropdown = Dropdown(&participant_names_dropdown, &selected_participant_idx);

    auto btn_assign = Button("Assign", [&] {
        if (task_id.empty() || localParticipants.empty()) return;
        if (selected_participant_idx >= 0 && selected_participant_idx < (int)localParticipants.size()) {
            try { 
                system.assignTask(task_id, localParticipants[selected_participant_idx]); 
                message_buffer = "Assigned Task " + task_id;
                logEvent("Assigned Task " + task_id + " to " + localParticipants[selected_participant_idx]->getName());
            } catch (const std::exception& e) { message_buffer = std::string("Error: ") + e.what(); }
            catch (...) { message_buffer = "Error: Unknown exception on Assign"; }
        }
    });

    auto btn_start = Button("Start", [&] { 
        if(task_id.empty()) return;
        try { 
            system.startTask(task_id); 
            message_buffer = "Started Task " + task_id;
            logEvent("Started Task " + task_id); 
        } catch (const std::exception& e) { message_buffer = std::string("Error: ") + e.what(); }
        catch (...) { message_buffer = "Error: Unknown exception on Start"; }
    });

    auto btn_complete = Button("Complete", [&] { 
        if(task_id.empty()) return;
        try { 
            system.completeTask(task_id); 
            message_buffer = "Completed Task " + task_id;
            logEvent("Completed Task " + task_id); 
        } catch (const std::exception& e) { message_buffer = std::string("Error: ") + e.what(); }
        catch (...) { message_buffer = "Error: Unknown exception on Complete"; }
    });

    auto btn_start_complete = Button("Start & Complete", [&] {
        if(task_id.empty()) return;
        try {
            system.startTask(task_id);
            system.completeTask(task_id);
            message_buffer = "Started & Completed " + task_id;
            logEvent("Started & Completed Task " + task_id);
        } catch (const std::exception& e) { message_buffer = std::string("Error: ") + e.what(); }
        catch (...) { message_buffer = "Error: Unknown exception on Start&Complete"; }
    });

    auto btn_reject = Button("Reject", [&] { 
        if(task_id.empty()) return;
        try { 
            system.rejectTask(task_id); 
            message_buffer = "Rejected Task " + task_id;
            logEvent("Rejected Task " + task_id); 
        } catch (const std::exception& e) { message_buffer = std::string("Error: ") + e.what(); }
        catch (...) { message_buffer = "Error: Unknown exception on Reject"; }
    });

    auto btn_undo = Button("Undo Last Action", [&] { 
        try { 
            system.undoLast(); 
            message_buffer = "Undid last command";
            logEvent("Undid Last Command"); 
        } catch (const std::exception& e) { message_buffer = std::string("Error: ") + e.what(); }
        catch (...) { message_buffer = "Error: Unknown exception on Undo"; }
    });

    auto action_row1 = Container::Horizontal(std::vector<Component>{ btn_start, btn_complete, btn_reject });
    auto action_row2 = Container::Horizontal(std::vector<Component>{ btn_start_complete, btn_undo });
    auto controls = Container::Vertical(std::vector<Component>{ in_task, action_row1, action_row2, dropdown, btn_assign });

    return Renderer(controls, [in_task, action_row1, action_row2, dropdown, btn_assign, this] {
        return vbox({
            text("Task Command Center") | bold | color(Color::Green),
            separator(),
            text("Enter Task ID:"),
            in_task->Render(),
            action_row1->Render(),
            action_row2->Render(),
            separator(),
            text("Assign Task to Participant:"),
            dropdown->Render(),
            btn_assign->Render(),
            separator(),
            text(message_buffer) | color(Color::Yellow) 
        }) | flex;
    });
}

Component TividyGUI::buildAuditTab() {
    auto list = Make<ScrollableList>(&audit_lines, &audit_selected, 15);
    return Renderer(list, [this, list] {
        refreshAudit();
        return vbox({
            text("System Audit Log") | bold | color(Color::Green),
            text("(Saved to tividy_audit_log.txt)  ·  ↑/↓ to scroll") | dim,
            separator(),
            list->Render() | flex
        }) | flex;
    });
}

std::vector<std::string> TividyGUI::pendingTasks(const std::string& instance_id, const std::set<std::string>& skip) {
    std::set<std::string> done;
    std::vector<std::string> assigned;
    try {
        for (const std::string& line : system.getHistory()) {
            if (!instance_id.empty() && line.find(instance_id) == std::string::npos) continue;

            if (line.find("task_assigned") != std::string::npos) {
                size_t at = line.find("node ");
                if (at != std::string::npos) {
                    size_t start = at + 5;
                    size_t end = line.find(')', start);
                    if (end != std::string::npos) assigned.push_back(line.substr(start, end - start));
                }
            } else if (line.find("] completed") != std::string::npos) {
                size_t at = line.find("node ");
                if (at != std::string::npos) {
                    size_t start = at + 5;
                    size_t end = line.find(')', start);
                    if (end != std::string::npos) done.insert(line.substr(start, end - start));
                }
            }
        }
    } catch(...) {}
    std::vector<std::string> pending;
    for (const std::string& id : assigned) {
        if (!done.count(id) && !skip.count(id)) pending.push_back(id);
    }
    return pending;
}

bool TividyGUI::finished(const std::string& instance_id) {
    try {
        for (const std::string& line : system.getHistory()) {
            if (line.find("instance_completed") != std::string::npos) {
                if (instance_id.empty() || line.find(instance_id) != std::string::npos) return true;
            }
        }
    } catch(...) {}
    return false;
}

void TividyGUI::runUniversityDemo() {
    logEvent("STARTED UNIVERSITY DEMO SCRIPT");
    if (available_definitions.empty() || available_definitions[0] == "(No definitions loaded)") return;
    try {
        WorkflowData data;
        data.set("name", "Goliath Junior");
        data.set("height", "3.1");
        std::string id = system.startWorkflow(available_definitions[0], data);
        
        if (id != "Definition not found" && !id.empty()) {
            localInstanceIds.push_back(id);
            logEvent("Started Instance " + id);
            
            std::set<std::string> skipped;
            for (int guard = 0; guard < 50 && !finished(id); guard++) {
                std::vector<std::string> pending = pendingTasks(id, skipped);
                if (pending.empty()) break;
                std::string next_task = pending[0];

                bool success = false;
                try { system.startTask(next_task); success = true; } 
                catch (const std::exception& e) { 
                    if (std::string(e.what()).find("Completed") != std::string::npos) skipped.insert(next_task);
                }
                if (success) {
                    try { system.completeTask(next_task); } 
                    catch (const std::exception& e) {
                        if (std::string(e.what()).find("Completed") != std::string::npos) skipped.insert(next_task);
                    }
                }
            }
            logEvent("FINISHED UNIVERSITY DEMO SCRIPT");
        }
    } catch (const std::exception& e) { 
        logEvent(std::string("CRASH CAUGHT IN UNIVERSITY DEMO: ") + e.what()); 
    } catch (...) { logEvent("CRASH CAUGHT IN UNIVERSITY DEMO"); }
}

void TividyGUI::runInsuranceDemo() {
    logEvent("STARTED INSURANCE DEMO SCRIPT");
    if (available_definitions.size() < 2 || available_definitions[1] == "(No definitions loaded)") return;
    try {
        WorkflowData data;
        data.set("name", "Sam Small");
        data.set("amount", "15000");
        std::string id = system.startWorkflow(available_definitions[1], data);
        
        if (id != "Definition not found" && !id.empty()) {
            localInstanceIds.push_back(id);
            logEvent("Started Instance " + id);

            std::set<std::string> skipped;
            for (int guard = 0; guard < 50; guard++) {
                std::vector<std::string> pending = pendingTasks(id, skipped);
                bool found_n7 = false;
                for (const auto& pid : pending) if (pid == "n7") found_n7 = true;
                if (found_n7) break;

                if (pending.empty()) break;
                std::string next_task = pending[0];

                logEvent("Loop " + std::to_string(guard) + " on " + next_task);
                bool success = false;
                try { system.startTask(next_task); success = true; } 
                catch (const std::exception& e) { 
                    if (std::string(e.what()).find("Completed") != std::string::npos) skipped.insert(next_task);
                    else logEvent("  startTask THREW: " + std::string(e.what()));
                }
                if (success) {
                    try { system.completeTask(next_task); } 
                    catch (const std::exception& e) { 
                        if (std::string(e.what()).find("Completed") != std::string::npos) skipped.insert(next_task);
                        else logEvent("  completeTask THREW: " + std::string(e.what()));
                    }
                }
            }

            
            std::vector<std::string> pending_now = pendingTasks(id, skipped);
            bool n7_exists = false;
            for (const auto& p : pending_now) if (p == "n7") n7_exists = true;
            
            if (n7_exists) {
                logEvent("Executing n7 Reject/Undo Test");
                try { system.startTask("n7"); } catch(...) {}
                try { system.rejectTask("n7"); } catch(...) {}
                try { system.completeTask("n7"); } catch(...) {}
            } else {
                logEvent("n7 never assigned. Pending: ");
                for (const auto& p : pending_now) logEvent("  pending: " + p);
            }

            
            for (int guard = 0; guard < 50 && !finished(id); guard++) {
                std::vector<std::string> pending = pendingTasks(id, skipped);
                if (pending.empty()) break;
                std::string next_task = pending[0];

                logEvent("Final loop " + std::to_string(guard) + " on " + next_task);
                bool success = false;
                try { system.startTask(next_task); success = true; } 
                catch (const std::exception& e) { 
                    if (std::string(e.what()).find("Completed") != std::string::npos) skipped.insert(next_task);
                }
                if (success) {
                    try { system.completeTask(next_task); } 
                    catch (const std::exception& e) {
                        if (std::string(e.what()).find("Completed") != std::string::npos) skipped.insert(next_task);
                    }
                }
            }
            logEvent("FINISHED INSURANCE DEMO SCRIPT (finished flag=" + std::to_string(finished(id)) + ")");
        }
    } catch (const std::exception& e) { 
        logEvent(std::string("CRASH CAUGHT IN INSURANCE DEMO: ") + e.what()); 
    } catch (...) { 
        logEvent("CRASH CAUGHT IN INSURANCE DEMO"); 
    }
}