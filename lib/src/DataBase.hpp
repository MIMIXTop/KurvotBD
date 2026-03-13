#pragma once
#include <memory>
#include <pqxx/pqxx>


class DataBase {
public:
    DataBase(const std::string &connectinString);

    std::string test();

    ~DataBase();

private:
    pqxx::connection connection;
};
