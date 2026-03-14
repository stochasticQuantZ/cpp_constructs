#include "database/sqlite_db.hpp"

#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

int main() {
    fs::create_directories("data");

    SqliteDb db("data/test.db");
    db.open();

    db.exec(R"(
        DROP TABLE IF EXISTS employees;
    )");

    db.exec(R"(
        CREATE TABLE employees (
            id INTEGER PRIMARY KEY,
            name TEXT NOT NULL,
            department TEXT NOT NULL,
            salary REAL NOT NULL
        );
    )");

    db.exec(R"(
        INSERT INTO employees (id, name, department, salary) VALUES
        (1, 'Alice', 'Engineering', 120000.0),
        (2, 'Bob', 'Finance', 95000.0),
        (3, 'Charlie', 'HR', 85000.0);
    )");

    std::cout << "Database created and seeded at data/test.db\n";
    return 0;
}