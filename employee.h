#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include <string>
#include <vector>

struct Employee {
    int id;
    std::string fullName;
    float hoursPerWeek;
    std::vector<int> assignedTaskIds;
    
    Employee();
    Employee(int id, const std::string& name, float hoursPerWeek);
    
    std::string serialize() const;
    static Employee deserialize(const std::string& data);
};

#endif
