#include "WorkflowEvent.h"
#include <ctime>
#include <iomanip>
#include <sstream>

WorkflowEvent::WorkflowEvent(std::string type, std::string instanceId, std::string nodeId,
                             std::string time)
    : type_(std::move(type)), instanceId_(std::move(instanceId)),
      nodeId_(std::move(nodeId)), time_(std::move(time)) {}

std::string WorkflowEvent::currentTimestamp() {
//     std::time_t now = std::time(nullptr);
//     std::tm local{};
// #ifdef _WIN32
//     localtime_s(&local, &now);
// #else
//     localtime_r(&now, &local);
// #endif
//     std::ostringstream out;
//     out << std::put_time(&local, "%Y-%m-%d %H:%M:%S");
//     return out.str();

    return "";
}