#ifndef DATABASE_H
#define DATABASE_H

#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <iostream>

template<typename T>
class Database {
private:
    std::vector<T> items;
    std::string filename;
    
    virtual std::string serialize(const T& item) const = 0;
    virtual T deserialize(const std::string& data) const = 0;
    
public:
    Database(const std::string& file) : filename(file) {}
    
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
    
    void save() const {
        std::ofstream out(filename);
        for (const auto& item : items) {
            out << serialize(item) << std::endl;
        }
        out.close();
    }
    
    void add(const T& item) {
        items.push_back(item);
        save();
    }
    
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
    
    T* find(int id) {
        auto it = std::find_if(items.begin(), items.end(),
            [id](const T& item) { return item.id == id; });
        if (it != items.end()) {
            return &(*it);
        }
        return nullptr;
    }
    
    std::vector<T>& getAll() { return items; }
    const std::vector<T>& getAll() const { return items; }
    
    size_t size() const { return items.size(); }
};

class TaskDB : public Database<Task> {
protected:
    std::string serialize(const Task& item) const override {
        return item.serialize();
    }
    Task deserialize(const std::string& data) const override {
        return Task::deserialize(data);
    }
    
public:
    TaskDB(const std::string& file) : Database<Task>(file) {}
};

class EmployeeDB : public Database<Employee> {
protected:
    std::string serialize(const Employee& item) const override {
        return item.serialize();
    }
    Employee deserialize(const std::string& data) const override {
        return Employee::deserialize(data);
    }
    
public:
    EmployeeDB(const std::string& file) : Database<Employee>(file) {}
};

#endif
