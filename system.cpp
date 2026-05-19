
#include "system.h"
#include <iostream>
#include <iomanip>

WorkTimeSystem::WorkTimeSystem() 
    : taskDB(std::make_unique<TaskDB>("tasks.txt")),
      employeeDB(std::make_unique<EmployeeDB>("employees.txt")),
      nextTaskId(1), nextEmployeeId(1) {
    loadData();
}

void WorkTimeSystem::loadData() {
    taskDB->load();
    employeeDB->load();
    
    for (const auto& task : taskDB->getAll()) {
        if (task.id >= nextTaskId) nextTaskId = task.id + 1;
    }
    for (const auto& emp : employeeDB->getAll()) {
        if (emp.id >= nextEmployeeId) nextEmployeeId = emp.id + 1;
    }
}

void WorkTimeSystem::addEmployee(const std::string& name, float hoursPerWeek) {
    Employee emp(nextEmployeeId++, name, hoursPerWeek);
    employeeDB->add(emp);
    std::cout << "Сотрудник добавлен. ID: " << emp.id << std::endl;
}

void WorkTimeSystem::addTask(const std::string& desc, float hoursRequired, int deadlineHours) {
    Task task(nextTaskId++, desc, hoursRequired, deadlineHours);
    taskDB->add(task);
    std::cout << "Задача добавлена. ID: " << task.id << std::endl;
}

void WorkTimeSystem::removeEmployee(int id) {
    Employee* emp = employeeDB->find(id);
    if (!emp) {
        std::cout << "Сотрудник не найден." << std::endl;
        return;
    }
    
    for (int taskId : emp->assignedTaskIds) {
        Task* task = taskDB->find(taskId);
        if (task) {
            task->status = "не начата";
        }
    }
    
    employeeDB->remove(id);
    taskDB->save();
    std::cout << "Сотрудник удалён." << std::endl;
}

void WorkTimeSystem::removeTask(int id) {
    Task* task = taskDB->find(id);
    if (!task) {
        std::cout << "Задача не найдена." << std::endl;
        return;
    }
    
    for (auto& emp : employeeDB->getAll()) {
        auto it = std::find(emp.assignedTaskIds.begin(), emp.assignedTaskIds.end(), id);
        if (it != emp.assignedTaskIds.end()) {
            emp.assignedTaskIds.erase(it);
        }
    }
    
    taskDB->remove(id);
    employeeDB->save();
    std::cout << "Задача удалена." << std::endl;
}

void WorkTimeSystem::assignTask(int taskId, int employeeId) {
    Task* task = taskDB->find(taskId);
    Employee* emp = employeeDB->find(employeeId);
    
    if (!task || !emp) {
        std::cout << "Задача или сотрудник не найдены." << std::endl;
        return;
    }
    
    auto it = std::find(emp->assignedTaskIds.begin(), emp->assignedTaskIds.end(), taskId);
    if (it != emp->assignedTaskIds.end()) {
        std::cout << "Эта задача уже назначена сотруднику." << std::endl;
        return;
    }
    
    emp->assignedTaskIds.push_back(taskId);
    if (task->status == "не начата") {
        task->status = "в процессе";
    }
    
    employeeDB->save();
    taskDB->save();
    
    std::cout << "Задача ID " << taskId << " назначена сотруднику ID " << employeeId << std::endl;
}

void WorkTimeSystem::unassignTask(int taskId, int employeeId) {
    Employee* emp = employeeDB->find(employeeId);
    Task* task = taskDB->find(taskId);
    
    if (!emp || !task) {
        std::cout << "Сотрудник или задача не найдены." << std::endl;
        return;
    }
    
    auto it = std::find(emp->assignedTaskIds.begin(), emp->assignedTaskIds.end(), taskId);
    if (it != emp->assignedTaskIds.end()) {
        emp->assignedTaskIds.erase(it);
        task->status = "не начата";
        
        employeeDB->save();
        taskDB->save();
        std::cout << "Задача снята с сотрудника." << std::endl;
    } else {
        std::cout << "Задача не назначена этому сотруднику." << std::endl;
    }
}

void WorkTimeSystem::viewAllEmployees() const {
    std::cout << "\n=== СПИСОК СОТРУДНИКОВ ===" << std::endl;
    for (const auto& emp : employeeDB->getAll()) {
        std::cout << "ID: " << emp.id << ", ФИО: " << emp.fullName

<< ", Часов в неделю: " << emp.hoursPerWeek 
                  << ", Задач: " << emp.assignedTaskIds.size() << std::endl;
        for (int taskId : emp.assignedTaskIds) {
            Task* task = taskDB->find(taskId);
            if (task) {
                std::cout << "  - Задача ID " << taskId << ": " << task->description 
                          << " [" << task->status << "]" << std::endl;
            }
        }
    }
}

void WorkTimeSystem::viewAllTasks() const {
    std::cout << "\n=== СПИСОК ЗАДАЧ ===" << std::endl;
    for (const auto& task : taskDB->getAll()) {
        std::cout << "ID: " << task.id << ", Описание: " << task.description 
                  << ", Трудоёмкость: " << task.hoursSpent << "/" << task.hoursRequired 
                  << " ч., Срок: " << task.deadlineHours << " ч."
                  << ", Статус: " << task.status << std::endl;
    }
}

void WorkTimeSystem::viewEmployeeTasks(int employeeId) const {
    Employee* emp = employeeDB->find(employeeId);
    if (!emp) {
        std::cout << "Сотрудник не найден." << std::endl;
        return;
    }
    
    std::cout << "\nЗадачи сотрудника " << emp->fullName << ":" << std::endl;
    for (int taskId : emp->assignedTaskIds) {
        Task* task = taskDB->find(taskId);
        if (task) {
            std::cout << "  ID " << task->id << ": " << task->description 
                      << " [" << task->status << "]" << std::endl;
        }
    }
}

void WorkTimeSystem::advanceTime(int hours) {
    std::cout << "Прокручиваем время на " << hours << " часов..." << std::endl;
    
    for (auto& emp : employeeDB->getAll()) {
        if (emp.assignedTaskIds.empty()) continue;
        
        float workTimeThisPeriod = (emp.hoursPerWeek / 168.0f) * hours;
        float timePerTask = workTimeThisPeriod / emp.assignedTaskIds.size();
        
        for (int taskId : emp.assignedTaskIds) {
            Task* task = taskDB->find(taskId);
            if (task && task->status == "в процессе") {
                task->hoursSpent += timePerTask;
                if (task->hoursSpent >= task->hoursRequired) {
                    task->status = "готова";
                }
            }
        }
    }
    
    for (auto& task : taskDB->getAll()) {
        if (task.status != "готова" && task.status != "просрочена") {
            task.deadlineHours -= hours;
            if (task.deadlineHours <= 0 && task.status != "готова") {
                task.status = "просрочена";
            }
        }
    }
    
    saveData();
    std::cout << "Время прокручено. Статусы задач обновлены." << std::endl;
}

void WorkTimeSystem::updateTaskStatus() {
    for (auto& task : taskDB->getAll()) {
        if (task.status == "в процессе" && task.hoursSpent >= task.hoursRequired) {
            task.status = "готова";
        }
        if (task.status != "готова" && task.status != "просрочена" && task.deadlineHours <= 0) {
            task.status = "просрочена";
        }
    }
    saveData();
}

void WorkTimeSystem::saveData() {
    taskDB->save();
    employeeDB->save();
}

void WorkTimeSystem::runMenu() {
    while (true) {
        std::cout << "\n===== МЕНЮ =====" << std::endl;
        std::cout << "1. Посмотреть всех сотрудников" << std::endl;
        std::cout << "2. Посмотреть все задачи" << std::endl;
        std::cout << "3. Добавить сотрудника" << std::endl;
        std::cout << "4. Добавить задачу" << std::endl;
        std::cout << "5. Удалить сотрудника" << std::endl;
        std::cout << "6. Удалить задачу" << std::endl;
        std::cout << "7. Назначить задачу сотруднику" << std::endl;
        std::cout << "8. Снять задачу с сотрудника" << std::endl;
        std::cout << "9. Просмотреть задачи сотрудника" << std::endl;
        std::cout << "10. Прокрутить время (24 часа)" << std::endl;

std::cout << "0. Выход" << std::endl;
        std::cout << "Выберите действие: ";
        
        int choice;
        std::cin >> choice;
        std::cin.ignore();
        
        switch (choice) {
            case 1: viewAllEmployees(); break;
            case 2: viewAllTasks(); break;
            case 3: {
                std::string name;
                float hours;
                std::cout << "Введите ФИО: ";
                std::getline(std::cin, name);
                std::cout << "Введите количество часов в неделю: ";
                std::cin >> hours;
                addEmployee(name, hours);
                break;
            }
            case 4: {
                std::string desc;
                float hoursReq;
                int deadline;
                std::cout << "Введите описание задачи: ";
                std::getline(std::cin, desc);
                std::cout << "Введите трудоёмкость (часов): ";
                std::cin >> hoursReq;
                std::cout << "Введите срок (часов): ";
                std::cin >> deadline;
                addTask(desc, hoursReq, deadline);
                break;
            }
            case 5: {
                int id;
                std::cout << "Введите ID сотрудника: ";
                std::cin >> id;
                removeEmployee(id);
                break;
            }
            case 6: {
                int id;
                std::cout << "Введите ID задачи: ";
                std::cin >> id;
                removeTask(id);
                break;
            }
            case 7: {
                int taskId, empId;
                std::cout << "Введите ID задачи: ";
                std::cin >> taskId;
                std::cout << "Введите ID сотрудника: ";
                std::cin >> empId;
                assignTask(taskId, empId);
                break;
            }
            case 8: {
                int taskId, empId;
                std::cout << "Введите ID задачи: ";
                std::cin >> taskId;
                std::cout << "Введите ID сотрудника: ";
                std::cin >> empId;
                unassignTask(taskId, empId);
                break;
            }
            case 9: {
                int id;
                std::cout << "Введите ID сотрудника: ";
                std::cin >> id;
                viewEmployeeTasks(id);
                break;
            }
            case 10: {
                advanceTime(24);
                break;
            }
            case 0:
                std::cout << "Выход..." << std::endl;
                return;
            default:
                std::cout << "Неверный выбор." << std::endl;
        }
    }
}
