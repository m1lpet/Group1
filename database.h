/**
 * @file database.h
 * @brief Шаблонный абстрактный класс для работы с базой данных
 * 
 * Предоставляет базовый функционал CRUD (Create, Read, Update, Delete)
 * для хранения объектов в текстовом файле. Каждая запись хранится в отдельной строке.
 * 
 * @tparam T Тип объектов, хранящихся в базе. Должен иметь поле id и методы serialize()/deserialize()
 * @note Класс является абстрактным: необходимо переопределить методы serialize() и deserialize()
 */

#ifndef DATABASE_H
#define DATABASE_H

#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <iostream>

/**
 * @brief Шаблонный класс базы данных
 * 
 * @tparam T Тип хранимых объектов
 */
template<typename T>
class Database {
private:
    std::vector<T> items;      ///< Вектор хранящихся объектов
    std::string filename;      ///< Путь к файлу для сохранения данных
    
    /**
     * @brief Сериализует объект в строку (чисто виртуальный метод)
     * @param item Объект для сериализации
     * @return Строковое представление объекта
     */
    virtual std::string serialize(const T& item) const = 0;
    
    /**
     * @brief Десериализует объект из строки (чисто виртуальный метод)
     * @param data Строка из файла
     * @return Восстановленный объект
     */
    virtual T deserialize(const std::string& data) const = 0;
    
public:
    /**
     * @brief Конструктор базы данных
     * @param file Путь к файлу для хранения данных
     */
    Database(const std::string& file) : filename(file) {}
    
    /**
     * @brief Загружает данные из файла в память
     * 
     * Читает файл построчно и десериализует каждую непустую строку в объект.
     * Если файл не существует, метод просто возвращает управление.
     * 
     * @warning Предыдущие данные в векторе НЕ очищаются — повторный вызов load()
     *          приведёт к дублированию записей.
     */
    void load() {
        std::ifstream in(filename);
        if (!in.is_open()) return;
        
        std::string line;
        while (std::getline(in, line)) {
            if (!line.empty()) {
                items.push_back(deserialize(line));
            }
        }
        in.close();
    }
    
    /**
     * @brief Сохраняет текущее состояние базы данных в файл
     * 
     * Перезаписывает файл, записывая каждый объект в отдельную строку
     */
    void save() const {
        std::ofstream out(filename);
        for (const auto& item : items) {
            out << serialize(item) << std::endl;
        }
        out.close();
    }
    
    /**
     * @brief Добавляет новый объект в базу данных
     * @param item Добавляемый объект
     * @note Автоматически сохраняет изменения в файл
     */
    void add(const T& item) {
        items.push_back(item);
        save();
    }
    
    /**
     * @brief Удаляет объект по идентификатору
     * @param id Идентификатор удаляемого объекта
     * @return true если объект найден и удалён, false если объект с таким id не найден
     * @note Автоматически сохраняет изменения в файл
     */
    bool remove(int id) {
        auto it = std::find_if(items.begin(), items.end(),
            [id](const T& item) { return item.id == id; });
        if (it != items.end()) {
            items.erase(it);
            save();
            return true;
        }
        return false;
    }
    
    /**
     * @brief Находит объект по идентификатору
     * @param id Идентификатор объекта
     * @return Указатель на найденный объект или nullptr, если не найден
     * @warning Возвращает указатель на внутренние данные — изменение объекта
     *          через этот указатель изменит данные в базе, но для сохранения
     *          в файл потребуется вызвать save() вручную.
     */
    T* find(int id) {
        auto it = std::find_if(items.begin(), items.end(),
            [id](const T& item) { return item.id == id; });
        if (it != items.end()) {
            return &(*it);
        }
        return nullptr;
    }
    
    /**
     * @brief Возвращает ссылку на вектор всех объектов
     * @return Ссылка на вектор объектов
     */
    std::vector<T>& getAll() { return items; }
    
    /**
     * @brief Константная версия getAll()
     * @return Константная ссылка на вектор объектов
     */
    const std::vector<T>& getAll() const { return items; }
    
    /**
     * @brief Возвращает количество объектов в базе
     * @return Размер базы данных
     */
    size_t size() const { return items.size(); }
    
    /**
     * @brief Проверяет, пуста ли база данных
     * @return true если база не содержит ни одного объекта
     */
    bool empty() const { return items.empty(); }
    
    /**
     * @brief Очищает все данные из памяти (без удаления файла)
     * @warning Чтобы сохранить очистку, после вызова нужно вызвать save() вручную
     */
    void clear() { items.clear(); }
    
    /**
     * @brief Перезагружает базу данных из файла (очищает память + загружает)
     */
    void reload() {
        items.clear();
        load();
    }
};

/**
 * @brief Класс базы данных для задач (Task)
 * 
 * Специализация шаблонного класса Database для работы с объектами Task.
 * Реализует методы сериализации через соответствующие методы класса Task.
 */
class TaskDB : public Database<Task> {
protected:
    /**
     * @brief Сериализует задачу в строку
     * @param item Объект Task
     * @return Строка, полученная из Task::serialize()
     */
    std::string serialize(const Task& item) const override {
        return item.serialize();
    }
    
    /**
     * @brief Десериализует задачу из строки
     * @param data Строка из файла
     * @return Объект Task, восстановленный через Task::deserialize()
     */
    Task deserialize(const std::string& data) const override {
        return Task::deserialize(data);
    }
    
public:
    /**
     * @brief Конструктор базы данных задач
     * @param file Путь к файлу (например, "tasks.txt")
     */
    TaskDB(const std::string& file) : Database<Task>(file) {}
};

/**
 * @brief Класс базы данных для сотрудников (Employee)
 * 
 * Специализация шаблонного класса Database для работы с объектами Employee.
 * Реализует методы сериализации через соответствующие методы класса Employee.
 */
class EmployeeDB : public Database<Employee> {
protected:
    /**
     * @brief Сериализует сотрудника в строку
     * @param item Объект Employee
     * @return Строка, полученная из Employee::serialize()
     */
    std::string serialize(const Employee& item) const override {
        return item.serialize();
    }
    
    /**
     * @brief Десериализует сотрудника из строки
     * @param data Строка из файла
     * @return Объект Employee, восстановленный через Employee::deserialize()
     */
    Employee deserialize(const std::string& data) const override {
        return Employee::deserialize(data);
    }
    
public:
    /**
     * @brief Конструктор базы данных сотрудников
     * @param file Путь к файлу (например, "employees.txt")
     */
    EmployeeDB(const std::string& file) : Database<Employee>(file) {}
};

#endif // DATABASE_H
