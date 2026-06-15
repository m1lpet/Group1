/**
 * @file employee.h
 * @brief Класс сотрудника для системы управления задачами
 * 
 * Содержит структуру Employee, представляющую сотрудника,
 * и методы для работы с его данными, включая сериализацию.
 */

#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include <string>
#include <vector>

/**
 * @brief Структура, представляющая сотрудника
 * 
 * Хранит информацию о сотруднике и список ID назначенных ему задач.
 * Поддерживает сериализацию в строку и обратное восстановление.
 * 
 * @note Поле id должно быть уникальным для каждого сотрудника
 * @see Task
 */
struct Employee {
    int id;                          ///< Уникальный идентификатор сотрудника
    std::string fullName;            ///< Полное имя сотрудника
    float hoursPerWeek;              ///< Количество рабочих часов в неделю
    std::vector<int> assignedTaskIds; ///< Список ID задач, назначенных сотруднику
    
    /**
     * @brief Конструктор по умолчанию
     * @post id = 0, hoursPerWeek = 0, fullName и assignedTaskIds пусты
     */
    Employee();
    
    /**
     * @brief Конструктор с параметрами
     * @param id Уникальный идентификатор
     * @param name Полное имя
     * @param hoursPerWeek Часы работы в неделю
     */
    Employee(int id, const std::string& name, float hoursPerWeek);
    
    /**
     * @brief Сериализует сотрудника в строку
     * @return Формат: "id|fullName|hoursPerWeek|taskId1,taskId2,..."
     * 
     * @code
     * Employee e(5, "Иван Петров", 40);
     * e.assignedTaskIds = {1, 3, 7};
     * e.serialize(); // "5|Иван Петров|40|1,3,7"
     * @endcode
     */
    std::string serialize() const;
    
    /**
     * @brief Восстанавливает сотрудника из строки
     * @param data Строка в формате serialize()
     * @return Восстановленный объект Employee
     * @see serialize()
     */
    static Employee deserialize(const std::string& data);
};

#endif // EMPLOYEE_H
