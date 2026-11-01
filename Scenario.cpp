#include "Scenario.h"

#include <string>
#include <vector>

#include "StandardWorkflowBuilder.h"
#include "WorkflowDefinition.h"
#include "WorkflowDirector.h"
#include "RoleApprover.h"
#include "Participant.h"

WorkflowDefinition* buildGoliathUniversityWorkflow() {
    static RoleApprover headOfDegree("Head of Degree");
    static RoleApprover headOfFaculty("Head of Faculty");

    headOfDegree.setNext(&headOfFaculty);

    WorkflowDirector director("Goliath University Admission");
    director.stage("Admissions", false)
                .task("Check giant eligibility", "Admissions Officer")
                .stage("Document Checks", true)
                    .task("Verify ID", "Admissions Officer")
                    .task("Confirm Payment", "Admissions Officer")
                .end()
                .task("Review Application", "Head of Degree")
                .task("Send decision", "Admissions Officer")
            .end();

    StandardWorkflowBuilder builder;
    director.construct(builder);
    return new WorkflowDefinition(builder.getResult());
}


WorkflowDefinition* buildInsuranceClaimWorkflow() {
    WorkflowDirector director("Goliath Insurance Claim");
    director.stage("Claim Process", false)
                .stage("Intake", false)
                    .task("Capture claim details", "Claims Clerk")
                    .task("Verify policy is active", "Claims Clerk")
                .end()
                .stage("Assessment", true)
                    .task("Assess damage", "Assessor")
                    .task("Run fraud check", "Fraud Investigator")
                    .task("Check policy cover", "Claims Clerk")
                .end()
                .stage("Settlement", false)
                    .task("Approve payout", "Claims Manager")
                    .task("Pay claimant", "Finance Officer")
                    .task("Close claim", "Claims Clerk")
                .end()
            .end();

    StandardWorkflowBuilder builder;
    director.construct(builder);
    return new WorkflowDefinition(builder.getResult());
}

std::vector<Participant> scenarioParticipants() {
    return {
        Participant("uni-1", "Nathan", {"Admissions Officer"}),
        Participant("uni-2", "Chris", {"Head of Degree"}),
        Participant("uni-3", "Jack", {"Head of Faculty"}),
        Participant("ins-1", "Chris", {"Claims Clerk"}),
        Participant("ins-2", "Jack", {"Assessor"}),
        Participant("ins-3", "Nathan", {"Fraud Investigator"}),
        Participant("ins-4", "Simon", {"Claims Manager"}),
        Participant("ins-5", "Keagan", {"Finance Officer"}),
    };
}
