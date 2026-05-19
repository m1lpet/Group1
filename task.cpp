#include "task.h"
#include <sstream>

Task::Task() : id(0), hoursRequired(0), hoursSpent(0), deadlineHours(0), status("не начата") {}

Task::Task(int id, const std::string& desc, float hoursReq, int deadlineH)
    : id(id), description(desc), hoursRequired(hoursReq), hoursSpent(0), 
      deadlineHours(deadlineH), status("не начата") {}

std::string Task::serialize() const {
    std::ostringstream oss;
    oss << id << "|" << description << "|" << hoursRequired << "|" 
        << hoursSpent << "|" << deadlineHours << "|" << status;
    return oss.str();
}

Task Task::deserialize(const std::string& data) {
    std::istringstream iss(data);
    std::string token;
    Task t;
    
    std::getline(iss, token, '|');
    t.id = std::stoi(token);
    std::getline(iss, t.description, '|');
    std::getline(iss, token, '|');
    t.hoursRequired = std::stof(token);
    std::getline(iss, token, '|');
    t.hoursSpent = std::stof(token);
    std::getline(iss, token, '|');
    t.deadlineHours = std::stoi(token);
    std::getline(iss, t.status);
    
    return t;
} 
