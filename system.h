/**
 * @file system.h
 * @brief Класс WorkTimeSystem - основная логика приложения
 * 
 * Содержит класс WorkTimeSystem, который управляет сотрудниками и задачами,
 * обеспечивает назначение задач, прокрутку времени и обновление статусов.
 */

#ifndef SYSTEM_H
#define SYSTEM_H

#include "task.h"
#include "employee.h"
#include "database.h"
#include <memory>

/**
 * @brief Класс системы учёта рабочего времени
 * 
 * Основной класс приложения, который:
 * - управляет базами данных сотрудников и задач
 * - предоставляет интерфейс для работы через консольное меню
 * - автоматически обновляет статусы задач при прокрутке времени
 */
class WorkTimeSystem {
private:
    std::unique_ptr<TaskDB> taskDB;        ///< База данных задач
    std::unique_ptr<EmployeeDB> employeeDB; ///< База данных сотрудников
    int nextTaskId;      ///< Следующий доступный ID для новой задачи
    int nextEmployeeId;  ///< Следующий доступный ID для нового сотрудника
    
public:
    /**
     * @brief Конструктор системы учёта рабочего времени
     * 
     * Инициализирует базы данных с файлами "tasks.txt" и "employees.txt",
     * загружает существующие данные и устанавливает счётчики ID.
     */
    WorkTimeSystem();
    
    /**
     * @brief Добавляет нового сотрудника
     * @param name ФИО сотрудника
     * @param hoursPerWeek Количество рабочих часов в неделю
     */
    void addEmployee(const std::string& name, float hoursPerWeek);
    
    /**
     * @brief Добавляет новую задачу
     * @param desc Описание задачи
     * @param hoursRequired Требуемое количество часов
     * @param deadlineHours Срок выполнения в часах
     */
    void addTask(const std::string& desc, float hoursRequired, int deadlineHours);
    
    /**
     * @brief Удаляет сотрудника по ID
     * @param id Идентификатор сотрудника
     * @note При удалении сотрудника все его задачи становятся "не начаты"
     */
    void removeEmployee(int id);
    
    /**
     * @brief Удаляет задачу по ID
     * @param id Идентификатор задачи
     * @note При удалении задачи она снимается со всех сотрудников
     */
    void removeTask(int id);
    
    /**
     * @brief Назначает задачу сотруднику
     * @param taskId ID задачи
     * @param employeeId ID сотрудника
     * @note Если задача была "не начата", её статус меняется на "в процессе"
     */
    void assignTask(int taskId, int employeeId);
    
    /**
     * @brief Снимает задачу с сотрудника
     * @param taskId ID задачи
     * @param employeeId ID сотрудника
     * @note Задача становится "не начата"
     */
    void unassignTask(int taskId, int employeeId);
    
    /**
     * @brief Выводит список всех сотрудников с их задачами
     */
    void viewAllEmployees() const;
    
    /**
     * @brief Выводит список всех задач
     */
    void viewAllTasks() const;
    
    /**
     * @brief Выводит задачи конкретного сотрудника
     * @param employeeId ID сотрудника
     */
    void viewEmployeeTasks(int employeeId) const;
    
    /**
     * @brief Прокручивает время вперёд на заданное количество часов
     * 
     * Распределяет рабочее время между задачами сотрудников и обновляет
     * затраченные часы и статусы задач.
     * 
     * @param hours Количество часов для прокрутки
     */
    void advanceTime(int hours);
    
    /**
     * @brief Обновляет статусы всех задач
     * 
     * Проверяет завершённые и просроченные задачи и обновляет их статусы.
     */
    void updateTaskStatus();
    
    /**
     * @brief Запускает главное интерактивное меню
     * 
     * Обеспечивает взаимодействие с пользователем через консоль.
     */
    void runMenu();
    
private:
    /**
     * @brief Загружает данные из файлов
     * @note Вызывается в конструкторе
     */
    void loadData();
    
    /**
     * @brief Сохраняет данные в файлы
     * @note Вызывается автоматически при изменениях
     */
    void saveData();
};

#endif // SYSTEM_H
