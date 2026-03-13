#include "DataBase.hpp"

DataBase::DataBase(const std::string &connectionString)
    : connection(connectionString) {
}

DataBase::~DataBase() = default;

std::string DataBase::test() {
    return "Hello lox";
}
