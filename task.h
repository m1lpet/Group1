/**
 * @file task.h
 * @brief Класс задачи для системы управления задачами
 * 
 * Содержит структуру Task, представляющую задачу,
 * и методы для работы с её данными, включая сериализацию.
 */

#ifndef TASK_H
#define TASK_H

#include <string>
#include <vector>

/**
 * @brief Структура, представляющая задачу
 * 
 * Хранит информацию о задаче: описание, трудоёмкость, затраченное время,
 * дедлайн и текущий статус выполнения. Поддерживает сериализацию.
 * 
 * Возможные статусы:
 * - "не начата" — задача создана, но не назначена
 * - "в процессе" — задача назначена сотруднику
 * - "готова" — часы работы >= требуемых часов
 * - "просрочена" — дедлайн истёк, а задача не готова
 */
struct Task {
    int id;                    ///< Уникальный идентификатор задачи
    std::string description;   ///< Описание задачи
    float hoursRequired;       ///< Требуемое количество часов
    float hoursSpent;          ///< Уже затраченные часы
    int deadlineHours;         ///< Срок выполнения (часов от создания)
    std::string status;        ///< Статус: "не начата", "в процессе", "готова", "просрочена"
    
    /**
     * @brief Конструктор по умолчанию
     * @post id = 0, все поля нулевые, status = "не начата"
     */
    Task();
    
    /**
     * @brief Конструктор с параметрами
     * @param id Уникальный идентификатор
     * @param desc Описание задачи
     * @param hoursReq Требуемое количество часов
     * @param deadlineH Срок выполнения в часах
     */
    Task(int id, const std::string& desc, float hoursReq, int deadlineH);
    
    /**
     * @brief Сериализует задачу в строку
     * @return Строка в формате "id|description|hoursRequired|hoursSpent|deadlineHours|status"
     */
    std::string serialize() const;
    
    /**
     * @brief Восстанавливает задачу из строки
     * @param data Строка в формате serialize()
     * @return Восстановленный объект Task
     * @see serialize()
     */
    static Task deserialize(const std::string& data);
};

#endif // TASK_H
