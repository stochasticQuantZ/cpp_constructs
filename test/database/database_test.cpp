#include "database/sqlite_db.hpp"

#include <iostream>

int main() {
    SqliteDb db("data/test.db");
    db.open();

    auto employees = db.get_all_employees();

    std::cout << "Employee count: " << employees.size() << "\n";

    for (const auto& e : employees) {
        std::cout
            << "id=" << e.id
            << ", name=" << e.name
            << ", department=" << e.department
            << ", salary=" << e.salary
            << "\n";
    }

    return 0;
}