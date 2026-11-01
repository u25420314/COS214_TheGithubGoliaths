// #include <iostream>
// #include <vector>
// #include <string>
// #include <exception>

// #include "TividySystem.h"
// #include "WorkflowDefinition.h"
// #include "Scenario.h"

// using namespace std;

// string ask(const string& prompt) {
//     cout << prompt;
//     string input;
//     getline(cin, input);
//     return input;
// }

// void showHistory(TividySystem& system) {
//     vector<string> lines = system.getHistory();
//     cout << "\nHistory (" << lines.size() << " events):\n";
//     for (const string& line : lines) cout << "  " << line << "\n";
// }

// void runTextMenu(TividySystem& system, const vector<string>& workflows) {
//     cout << "=== Tividy System Menu ===\n";

//     while(true) {
//         cout << "\nMenu:\n";
//         cout << "  1. Start a workflow\n";
//         cout << "  2. Assign a task\n";
//         cout << "  3. Start a task\n";
//         cout << "  4. Complete a task\n";
//         cout << "  5. Reject a task\n";
//         cout << "  6. Undo last action\n";
//         cout << "  7. Show history\n";
//         cout << "  8. Exit\n";

//         string option = ask("> ");

//         try {   // invalid actions are refused by the task's state and reported here
//             if (option == "1") {
//                 for (size_t i = 0; i < workflows.size(); i++)
//                     cout << "  " << (i + 1) << ". " << workflows[i] << "\n";
//                 int pick = stoi(ask("Which workflow? ")) - 1;
//                 if (pick < 0 || pick >= (int)workflows.size()) { cout << "Invalid choice.\n"; continue; }

//                 WorkflowData data;
//                 data.set("name", ask("Name: "));
//                 string id = system.startWorkflow(workflows[pick], data);
//                 cout << "Started case " << id << ". Use 'Show history' to see which task was assigned.\n";
//             }
//             else if (option == "2") { system.startTask(ask("Task id (e.g. n2): "));    cout << "Done.\n"; }
//             else if (option == "3") { system.completeTask(ask("Task id (e.g. n2): ")); cout << "Done.\n"; }
//             else if (option == "4") { system.rejectTask(ask("Task id (e.g. n2): "));   cout << "Done.\n"; }
//             else if (option == "5") { system.undoLast(); cout << "Last action undone.\n"; }
//             else if (option == "6") showHistory(system);
//             else if (option == "0" || cin.eof()) break;
//             else cout << "Unknown option.\n";
//         } catch (const exception& e) {
//             cout << "Not allowed: " << e.what() << "\n";
//         }
//     }
//     cout << "Exiting Tividy System.\n";
// }

// int main() {
//     TividySystem system(scenarioParticipants());
//     vector<string> workflows;

//     WorkflowDefinition* university = buildGoliathUniversityWorkflow();
//     WorkflowDefinition* insurance = buildInsuranceClaimWorkflow();

//     workflows.push_back(university->getName());
//     workflows.push_back(insurance->getName());

//     system.registerDefinition(university);
//     system.registerDefinition(insurance);

//     runTextMenu(system, workflows);
//     return 0;
// }

/**
 * @file main.cpp
 * @brief TEMPORARY demo: runs both industries automatically, with no user input.
 *
 * It never guesses task ids: it reads the history to find which tasks the engine
 * has assigned, then starts and completes them, so it works with any workflow.
 * Each industry gets its own TividySystem so task ids from the two cases can't mix.
 */

#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <exception>

#include "TividySystem.h"
#include "WorkflowDefinition.h"
#include "Scenario.h"

using namespace std;

// the node id at the end of a history line, e.g. "... node n5)" -> "n5"
string nodeOf(const string& line) {
    size_t at = line.find("node ");
    if (at == string::npos) return "";
    size_t start = at + 5;
    size_t end = line.find(')', start);
    return line.substr(start, end - start);
}

// tasks the engine has assigned that are not completed yet, worked out from the history
vector<string> pendingTasks(TividySystem& system) {
    set<string> done;
    vector<string> assigned;
    for (const string& line : system.getHistory()) {
        if (line.find("task_assigned") != string::npos) assigned.push_back(nodeOf(line));
        else if (line.find("] completed") != string::npos) done.insert(nodeOf(line));
    }
    vector<string> pending;
    for (const string& id : assigned)
        if (!done.count(id)) pending.push_back(id);
    return pending;
}

bool finished(TividySystem& system) {
    for (const string& line : system.getHistory())
        if (line.find("instance_completed") != string::npos) return true;
    return false;
}

// runs one action and prints whether it worked or was refused
void step(const string& action, TividySystem& system, const string& taskId) {
    cout << "  - " << action << " " << taskId << ": ";
    try {
        if (action == "start") system.startTask(taskId);
        else if (action == "complete") system.completeTask(taskId);
        else if (action == "reject") system.rejectTask(taskId);
        cout << "ok\n";
    } catch (const exception& e) {
        cout << "REFUSED -> " << e.what() << "\n";
    }
}

// keeps completing whatever the engine assigns until the case is finished
void runToCompletion(TividySystem& system) {
    for (int guard = 0; guard < 50 && !finished(system); guard++) {
        vector<string> pending = pendingTasks(system);
        if (pending.empty()) { cout << "  (nothing left to do)\n"; return; }
        step("start", system, pending[0]);
        step("complete", system, pending[0]);
    }
}

// completes assigned tasks (except the target) until the target task has been assigned
void runUntilAssigned(TividySystem& system, const string& target) {
    for (int guard = 0; guard < 50; guard++) {
        vector<string> pending = pendingTasks(system);
        for (const string& id : pending) if (id == target) return;
        if (pending.empty()) return;
        step("start", system, pending[0]);
        step("complete", system, pending[0]);
    }
}

void showHistory(TividySystem& system) {
    vector<string> lines = system.getHistory();
    cout << "\n  History (" << lines.size() << " events):\n";
    for (const string& line : lines) cout << "    " << line << "\n";
}

// ---------- Industry 1: Goliath University (refused action + completion path) ----------
void runUniversity() {
    cout << "\n===== Industry 1: Goliath University Admissions =====\n";
    TividySystem system(scenarioParticipants());
    WorkflowDefinition* definition = buildGoliathUniversityWorkflow();
    string name = definition->getName();
    system.registerDefinition(definition);

    WorkflowData data;
    data.set("name", "Goliath Junior");
    data.set("height", "3.1");
    cout << "Started case " << system.startWorkflow(name, data) << " for Goliath Junior (3.1 m)\n";

    string first = pendingTasks(system).at(0);
    cout << "  Trying to complete " << first << " before starting it (should be refused):\n";
    step("complete", system, first);                       // State refuses this

    runToCompletion(system);
    showHistory(system);
}

// ---------- Industry 2: Goliath Insurance claim (exception path + undo) ----------
void runInsurance() {
    cout << "\n===== Industry 2: Goliath Insurance Claim =====\n";
    TividySystem system(scenarioParticipants());
    WorkflowDefinition* definition = buildInsuranceClaimWorkflow();
    string name = definition->getName();
    system.registerDefinition(definition);

    WorkflowData data;
    data.set("name", "Sam Small");
    data.set("amount", "15000");
    cout << "Started case " << system.startWorkflow(name, data) << " for Sam Small (R15000)\n";

    // work through intake until the parallel assessment stage assigns the fraud check (n7)
    runUntilAssigned(system, "n7");

    cout << "  Fraud check flags the claim: rejecting n7, then undoing it:\n";
    step("start", system, "n7");
    step("reject", system, "n7");                          // exception path
    system.undoLast();                                     // Command + Memento
    cout << "  - undo: ok (n7 is back in progress)\n";
    step("complete", system, "n7");

    runToCompletion(system);
    showHistory(system);
}

int main() {
    runUniversity();
    runInsurance();
    cout << "\nDemo finished.\n";
    return 0;
}