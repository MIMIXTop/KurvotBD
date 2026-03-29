#include "DataBase.hpp"

#include <iostream>
#include <format>

#include "DataBaseSourse/SqlRequests/MetaData.hpp"
#include "DataBaseSourse/SqlRequests/ModelMeta.hpp"
#include "util/util.hpp"

DataBase::DataBase(const std::string &connectionString)
    : connection(connectionString) {
}

DataBase::~DataBase() = default;

std::vector<std::string> DataBase::getTablesNames() {
    std::vector<std::string> tables;

    try {
        pqxx::work work(connection);

        pqxx::result res = work.exec(lib::Constants::getListTables);

        for (auto&& row : res) {
            tables.push_back(row[0].as<std::string>());
        }

        work.commit();
    } catch (std::exception &e) {
        std::println(std::cerr, "Error getting table list: {}", e.what());
    }

    return tables;
}

void DataBase::appendModel(lib::Models::Model &&model) {
    try {
        pqxx::work txn(connection);

        std::visit([&](auto&& m) {
            insertModel(txn, m);
        }, std::move(model));

        txn.commit();
    } catch (const std::exception& e) {
        std::println(std::cerr, "Error appending model: {}", e.what());
    }
}
