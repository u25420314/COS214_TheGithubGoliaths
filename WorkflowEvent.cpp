#include "WorkflowEvent.h"
#include <ctime>
#include <iomanip>
#include <sstream>

WorkflowEvent::WorkflowEvent(std::string type, std::string instanceId, std::string nodeId)
    : type(std::move(type)), instanceId(std::move(instanceId)), nodeId(std::move(nodeId)) {
    std::time_t now = std::time(nullptr);
    std::tm local{};
#ifdef _WIN32
    localtime_s(&local, &now);
#else
    localtime_r(&now, &local);
#endif
    std::ostringstream out;
    out << std::put_time(&local, "%Y-%m-%d %H:%M:%S");
    time = out.str();
}