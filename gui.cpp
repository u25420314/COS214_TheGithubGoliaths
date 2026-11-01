#include "gui.h"
#include "StandardWorkflowBuilder.h"
#include <algorithm>
#include <iostream>

using namespace ftxui;

TividyGUI::TividyGUI(TividySystem& sys, ftxui::ScreenInteractive& scr) : system(sys), screen(scr) {
    logEvent("TIVIDY Core Initialized.");

    tab_toggle = Radiobox(&tab_titles, &active_tab);

    // --- Participant Modal Setup ---
    auto in_p_id = Input(&p_id, "ID (e.g. p1)");
    auto in_p_name = Input(&p_name, "Name (e.g. Alice)");
    auto in_p_role = Input(&p_role, "Role (e.g. clerk)");
    
    auto btn_add_participant = Button("Register", [&] {
        if (p_id.empty() || p_name.empty()) return;
        Participant* p = new Participant(p_id, p_name, {p_role});
        localParticipants.push_back(p);
        updateParticipantDropdown();
        logEvent("Registered Participant: " + p_name);
        p_id = ""; p_name = ""; p_role = "";
        show_participant_modal = false;
    });

    // Fix: Explicitly build std::vector<Component>
    participant_modal = Container::Vertical(std::vector<Component>{
        Renderer([] { return text("Register New Participant") | bold | hcenter | color(Color::Cyan); }),
        Renderer([] { return separator(); }),
        in_p_id,
        in_p_name,
        in_p_role,
        Renderer([] { return separator(); }),
        Container::Horizontal(std::vector<Component>{
            Button("Cancel", [&] { show_participant_modal = false; }) | border,
            btn_add_participant | border | color(Color::Green)
        })
    }) | border | size(WIDTH, EQUAL, 50);

    // --- Build Tabs ---
    auto tab_container = Container::Tab({
        buildDashboardTab(), buildBuilderTab(), buildInstancesTab(), 
        buildTasksTab(), buildAuditTab()
    }, &active_tab);

    auto layout = Container::Horizontal(std::vector<Component>{
        tab_toggle,
        tab_container
    });

    auto main_ui = Renderer(layout, [this, tab_container] {
        return hbox(std::vector<Element>{
            // Sidebar
            vbox({
                text(" TIVIDY ") | bold | hcenter | color(Color::Cyan),
                separator(),
                tab_toggle->Render() | vscroll_indicator | frame | flex,
                separator(),
                text(" [Q]uit ") | dim | hcenter
            }) | border | size(WIDTH, EQUAL, 25),

            // Main Content
            vbox({
                tab_container->Render() | flex,
                separator(),
                text(" Logs: " + (auditLog.empty() ? "None" : auditLog.back())) | dim | color(Color::Yellow)
            }) | border | flex
        });
    });

    // Fix: Use Modal() to overlay the participant screen
    main_container = Modal(main_ui, participant_modal, &show_participant_modal);
    
    main_container |= CatchEvent([&](Event event) {
        if (event == Event::Character('q') || event == Event::Character('Q')) {
            screen.ExitLoopClosure()();
            return true;
        }
        return false;
    });
}

TividyGUI::~TividyGUI() {
    for (auto p : localParticipants) delete p;
    for (auto d : localDefinitions) delete d;
}

void TividyGUI::run() {
    screen.Loop(main_container);
}

void TividyGUI::logEvent(const std::string& msg) {
    auditLog.push_back(msg);
}

void TividyGUI::updateParticipantDropdown() {
    participant_names_dropdown.clear();
    for (auto p : localParticipants) {
        participant_names_dropdown.push_back(p->getName() + " (" + p->getId() + ")");
    }
    // Fix: cast to int to prevent sign-compare warning
    if (selected_participant_idx >= (int)participant_names_dropdown.size()) {
        selected_participant_idx = 0;
    }
}

Component TividyGUI::buildDashboardTab() {
    auto btn_reg_participant = Button("Register New Participant", [&] { show_participant_modal = true; });
    
    auto view = Container::Vertical(std::vector<Component>{
        Renderer([] { return text("System Overview") | bold | color(Color::Green); }),
        Renderer([] { return separator(); }),
        Renderer([this] {
            return vbox({
                text("Registered Definitions : " + std::to_string(localDefinitions.size())),
                text("Registered Participants: " + std::to_string(localParticipants.size())),
                text("Active Instances       : " + std::to_string(localInstanceIds.size())),
                text("Audit Log Entries      : " + std::to_string(auditLog.size())),
                filler(),
            });
        }),
        btn_reg_participant | border | color(Color::Cyan)
    });
    return view | flex;
}

Component TividyGUI::buildBuilderTab() {
    auto in_name = Input(&b_name, "Workflow Name");
    auto in_stage = Input(&b_stage_name, "Stage Name");
    auto in_task_name = Input(&b_task_name, "Task Name");
    auto in_task_role = Input(&b_task_role, "Required Role");
    auto cb_parallel = Checkbox("Parallel Stage", &b_is_parallel);

    auto btn_add_task = Button("Add Task to Stage", [&] {
        if (b_stage_name.empty() || b_task_name.empty()) return;
        auto it = std::find_if(b_stages.begin(), b_stages.end(), [&](const StageData& s){ return s.name == b_stage_name; });
        if (it == b_stages.end()) {
            b_stages.push_back({b_stage_name, b_is_parallel, {}});
            it = b_stages.end() - 1;
        }
        it->tasks.push_back({b_task_name, b_task_role});
        b_task_name = ""; b_task_role = "";
    });

    auto btn_finish = Button("Build & Register Workflow", [&] {
        if (b_name.empty() || b_stages.empty()) return;
        StandardWorkflowBuilder builder;
        builder.reset();
        for (const auto& stage : b_stages) {
            builder.beginStage(stage.name, stage.parallel);
            for (const auto& task : stage.tasks) {
                builder.addTask(task.first, task.second);
            }
            builder.endStage();
        }
        WorkflowDefinition* def = new WorkflowDefinition(builder.getResult());
        system.registerDefinition(def);
        localDefinitions.push_back(def);
        logEvent("Built & Registered: " + b_name);
        b_name = ""; b_stages.clear(); b_stage_name = "";
    });

    // Fix: wrap in std::vector<Component> to avoid the 16-item limit error
    std::vector<Component> items = {
        Renderer([] { return text("Workflow Builder") | bold | color(Color::Green); }),
        Renderer([] { return separator(); }),
        in_name | border,
        Renderer([] { return separator(); }),
        Renderer([] { return text("Stage Configuration"); }),
        in_stage | border,
        cb_parallel,
        Renderer([] { return separator(); }),
        Renderer([] { return text("Task Configuration"); }),
        in_task_name | border,
        in_task_role | border,
        btn_add_task | border,
        Renderer([] { return separator(); }),
        btn_finish | border | color(Color::Green),
        Renderer([] {return filler(); }),
        Renderer([this] {
            Elements els;
            els.push_back(text("Current Build Tree:") | underlined);
            for (const auto& s : b_stages) {
                els.push_back(text("Stage: " + s.name + (s.parallel ? " [Parallel]" : " [Sequential]")) | color(Color::Cyan));
                for (const auto& t : s.tasks) {
                    els.push_back(text("  └─ Task: " + t.first + " (Role: " + t.second + ")"));
                }
            }
            return vbox(std::move(els)) | border | size(HEIGHT, LESS_THAN, 10) | yframe;
        })
    };

    return Container::Vertical(items);
}

Component TividyGUI::buildInstancesTab() {
    auto in_def_name = Input(&inst_def_name, "Definition Name");
    auto btn_start = Button("Start Workflow", [&] {
        if (inst_def_name.empty()) return;
        WorkflowData data;
        std::string id = system.startWorkflow(inst_def_name, data);
        if (id == "Definition not found") {
            logEvent("ERROR: Definition not found.");
        } else {
            localInstanceIds.push_back(id);
            logEvent("Started Instance: " + id);
            inst_def_name = "";
        }
    });

    return Container::Vertical(std::vector<Component>{
        Renderer([] { return text("Instance Manager") | bold | color(Color::Green); }),
        Renderer([] { return separator(); }),
        Renderer([] { return text("Start a New Instance"); }),
        in_def_name | border,
        btn_start | border | color(Color::Cyan),
        Renderer([] { return separator(); }),
        Renderer([this] {
            Elements els;
            els.push_back(text("Active Instances:") | underlined);
            for (const auto& id : localInstanceIds) {
                els.push_back(text(" • ID: " + id) | color(Color::Yellow));
            }
            if (localInstanceIds.empty()) els.push_back(text("No instances running.") | dim);
            return vbox(std::move(els)) | border | flex;
        })
    });
}

Component TividyGUI::buildTasksTab() {
    auto in_task = Input(&task_id, "Task ID (e.g., node taskA)");
    auto dropdown = Dropdown(&participant_names_dropdown, &selected_participant_idx);

    auto btn_assign = Button("Assign Task", [&] {
        if (task_id.empty() || localParticipants.empty()) return;
        // Fix: cast to int to prevent warning
        if (selected_participant_idx >= 0 && selected_participant_idx < (int)localParticipants.size()) {
            Participant* p = localParticipants[selected_participant_idx];
            system.assignTask(task_id, p);
            logEvent("Assigned Task '" + task_id + "' to " + p->getName());
            task_id = "";
        }
    });

    return Container::Vertical(std::vector<Component>{
        Renderer([] { return text("Task Command Center") | bold | color(Color::Green); }),
        Renderer([] { return separator(); }),
        Renderer([] { return text("Assign Task"); }),
        in_task | border,
        Renderer([] { return text("Select Participant:"); }),
        dropdown | border,
        btn_assign | border | color(Color::Cyan),
        Renderer([] { return filler(); }), 
        Renderer([this] {
            if (localParticipants.empty()) return text("WARNING: Register participants first!") | color(Color::Red);
            return text("");
        })
    });
}

Component TividyGUI::buildAuditTab() {
    return Renderer([this] {
        Elements logs;
        logs.push_back(text("System Audit Log") | bold | color(Color::Green));
        logs.push_back(separator());
        int start = std::max(0, (int)auditLog.size() - 20);
        for (int i = start; i < (int)auditLog.size(); ++i) {
            logs.push_back(text(" [" + std::to_string(i+1) + "] " + auditLog[i]));
        }
        return vbox(std::move(logs)) | vscroll_indicator | frame | flex;
    });
}