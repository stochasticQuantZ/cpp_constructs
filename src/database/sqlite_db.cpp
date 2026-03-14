#include "database/sqlite_db.hpp"

#include <stdexcept>

SqliteDb::SqliteDb(const std::string& db_path) : db_path_(db_path) {}

SqliteDb::~SqliteDb() {
    close();
}

void SqliteDb::open() {
    if (sqlite3_open(db_path_.c_str(), &db_) != SQLITE_OK) {
        std::string err = db_ ? sqlite3_errmsg(db_) : "unknown error";
        throw std::runtime_error("Failed to open database: " + err);
    }
}

void SqliteDb::close() {
    if (db_) {
        sqlite3_close(db_);
        db_ = nullptr;
    }
}

void SqliteDb::exec(const std::string& sql) {
    char* err_msg = nullptr;
    int rc = sqlite3_exec(db_, sql.c_str(), nullptr, nullptr, &err_msg);
    if (rc != SQLITE_OK) {
        std::string err = err_msg ? err_msg : "unknown SQL error";
        sqlite3_free(err_msg);
        throw std::runtime_error("SQL exec failed: " + err);
    }
}

std::vector<Employee> SqliteDb::get_all_employees() {
    std::vector<Employee> result;

    const char* sql =
        "SELECT id, name, department, salary "
        "FROM employees ORDER BY id;";

    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db_, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        throw std::runtime_error("Failed to prepare query");
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        Employee e;
        e.id = sqlite3_column_int(stmt, 0);
        e.name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        e.department = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        e.salary = sqlite3_column_double(stmt, 3);
        result.push_back(e);
    }

    sqlite3_finalize(stmt);
    return result;
}