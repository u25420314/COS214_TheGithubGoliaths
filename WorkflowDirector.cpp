#include "WorkflowDirector.h"
#include "WorkflowBuilder.h"

//Constructor: stores the workflow name
WorkflowDirector::WorkflowDirector(const std::string& workflowName) {
    this->workflowName = workflowName;
}

//Destructor
WorkflowDirector::~WorkflowDirector() {}


//each record one step to run later and return the director so calls can be chained
WorkflowDirector& WorkflowDirector::stage(const std::string& name, bool parallel) {
    steps.push_back([name, parallel](WorkflowBuilder& b) { b.beginStage(name, parallel); });
    return *this;
}

//each record one step to run later and return the director so calls can be chained
WorkflowDirector& WorkflowDirector::task(const std::string& name, const std::string& role) {
    steps.push_back([name, role](WorkflowBuilder& b) { b.addTask(name, role); });
    return *this;
}

//each record one step to run later and return the director so calls can be chained
WorkflowDirector& WorkflowDirector::end() {
    steps.push_back([](WorkflowBuilder& b) { b.endStage(); });
    return *this;
}

//resets the builder, sets the name and replays all recorded steps on it
void WorkflowDirector::construct(WorkflowBuilder& builder) {
    builder.reset();
    builder.setWorkflowName(workflowName);
    for (size_t i = 0; i < steps.size(); i++) {
        steps[i](builder);
    }
}
