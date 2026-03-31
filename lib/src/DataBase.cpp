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

std::optional<lib::Models::Model> DataBase::getModelById(lib::Models::Model modelType, const std::string &id) {
    try {
        pqxx::work txn(connection);

        auto res = std::visit([&](auto&& m) {
            return selectModelById(txn, m, id);
        }, std::move(modelType));

        txn.commit();
        return res;
    } catch (const std::exception& e) {
        std::println(std::cerr, "Error getting model id: {}", e.what());
    }
    return std::nullopt;
}

void DataBase::deleteById(lib::Models::Model model, const std::string &id) {
    try {
        pqxx::work txn(connection);
        std::visit([&](auto&& m) { deleteModel(txn, m, id); }, std::move(model));
        txn.commit();
    } catch (const std::exception& e) {
        std::println(std::cerr, "Error deleting model id: {}", e.what());
    }
}

std::vector<lib::Models::Model> DataBase::selectAll(lib::Models::Model modelType) {
    try {
        pqxx::work txn(connection);

        auto res = std::visit([&](auto&& m) -> std::vector<lib::Models::Model> {
            auto vec = selectAllModels(txn, m);
            std::vector<lib::Models::Model> result;
            result.reserve(vec.size());
            for (auto& item : vec) {
                result.push_back(std::move(item));
            }
            return result;
        }, std::move(modelType));

        txn.commit();
        return res;
    } catch (const std::exception& e) {
        std::println(std::cerr, "Error selecting all models: {}", e.what());
    }
    return {};
}

void DataBase::updateById(lib::Models::Model model, const std::string &id) {
    try {
        pqxx::work txn(connection);
        std::visit([&](auto&& m) { updateModel(txn, m, id); }, std::move(model));
        txn.commit();
    } catch (const std::exception& e) {
        std::println(std::cerr, "Error updating model id: {}", e.what());
    }
}
