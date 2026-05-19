#include "employee.h"
#include <sstream>

Employee::Employee() : id(0), hoursPerWeek(0) {}

Employee::Employee(int id, const std::string& name, float hoursPerWeek)
    : id(id), fullName(name), hoursPerWeek(hoursPerWeek) {}

std::string Employee::serialize() const {
    std::ostringstream oss;
    oss << id << "|" << fullName << "|" << hoursPerWeek << "|";
    for (size_t i = 0; i < assignedTaskIds.size(); ++i) {
        if (i > 0) oss << ",";
        oss << assignedTaskIds[i];
    }
    return oss.str();
}

Employee Employee::deserialize(const std::string& data) {
    std::istringstream iss(data);
    std::string token;
    Employee e;
    
    std::getline(iss, token, '|');
    e.id = std::stoi(token);
    std::getline(iss, e.fullName, '|');
    std::getline(iss, token, '|');
    e.hoursPerWeek = std::stof(token);
    
    std::string tasksStr;
    std::getline(iss, tasksStr);
    if (!tasksStr.empty()) {
        std::istringstream tasksStream(tasksStr);
        std::string taskId;
        while (std::getline(tasksStream, taskId, ',')) {
            e.assignedTaskIds.push_back(std::stoi(taskId));
        }
    }
    
    return e;
}
