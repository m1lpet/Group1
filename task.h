#ifndef TASK_H
#define TASK_H

#include <string>
#include <vector>

struct Task {
    int id;
    std::string description;
    float hoursRequired;
    float hoursSpent;
    int deadlineHours;
    std::string status; // "не начата", "в процессе", "готова", "просрочена"
    
    Task();
    Task(int id, const std::string& desc, float hoursReq, int deadlineH);
    
    std::string serialize() const;
    static Task deserialize(const std::string& data);
};

#endif
