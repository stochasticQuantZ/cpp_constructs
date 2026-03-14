#pragma once

#include <sqlite3.h>
#include <string>
#include <vector>

struct Employee {
    int id;
    std::string name;
    std::string department;
    double salary;
};

class SqliteDb {
public:
    explicit SqliteDb(const std::string& db_path);
    ~SqliteDb();

    void open();
    void close();

    void exec(const std::string& sql);
    std::vector<Employee> get_all_employees();

private:
    std::string db_path_;
    sqlite3* db_ = nullptr;
};