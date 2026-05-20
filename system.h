#ifndef SYSTEM_H

#define SYSTEM_H



#include "task.h"

#include "employee.h"

#include "database.h"

#include <memory>



class WorkTimeSystem {

private:

    std::unique_ptr<TaskDB> taskDB;

    std::unique_ptr<EmployeeDB> employeeDB;

    int nextTaskId;

    int nextEmployeeId;

    

public:

    WorkTimeSystem();

    

    void addEmployee(const std::string& name, float hoursPerWeek);

    void addTask(const std::string& desc, float hoursRequired, int deadlineHours);

    void removeEmployee(int id);

    void removeTask(int id);

    void assignTask(int taskId, int employeeId);

    void unassignTask(int taskId, int employeeId);

    void viewAllEmployees() const;

    void viewAllTasks() const;

    void viewEmployeeTasks(int employeeId) const;

    void advanceTime(int days);

    void updateTaskStatus();

    void runMenu();

    

private:

    void loadData();

    void saveData();

};



#endif

