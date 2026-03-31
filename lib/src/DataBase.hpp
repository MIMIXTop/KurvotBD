#pragma once
#include <memory>
#include <pqxx/pqxx>

#include "Models.hpp"

#include "DataBaseSourse/SqlRequests/ModelMeta.hpp"
#include "DataBaseSourse/SqlRequests/AppendParam.hpp"


class DataBase {
public:
    DataBase(const std::string &connectinString);

    std::vector<std::string> getTablesNames();

    void appendModel(lib::Models::Model &&model);

    std::optional<lib::Models::Model> getModelById(lib::Models::Model modelType, const std::string &id);

    void deleteById(lib::Models::Model model, const std::string &id);

    std::vector<lib::Models::Model> selectAll(lib::Models::Model modelType);

    void updateById(lib::Models::Model model, const std::string &id);

    ~DataBase();

private:
    pqxx::connection connection;

    template <typename T>
    static void insertModel(pqxx::work& txn, const T& model) {
        auto values = lib::Meta::ModelMeta<T>::extract(model);
        std::apply([&](auto&&... args) {
            pqxx::params ps;
            (lib::Sql::appendParam(ps, args), ...);
            txn.exec(lib::Meta::ModelMeta<T>::Query::insert, ps);
        }, values);
    }

    template <typename T>
    static std::optional<lib::Models::Model> selectModelById(pqxx::work& txn, const T&, const std::string &id) {
        pqxx::params ps;
        ps.append(id);
        auto res = txn.exec(lib::Meta::ModelMeta<T>::Query::selectById, ps);

        if (res.empty()) {
            return std::nullopt;
        }

        return lib::Meta::ModelMeta<T>::fromRow(res[0]);
    }

    template <typename T>
    static void deleteModel(pqxx::work& txn, const T&, const std::string &id) {
        pqxx::params ps;
        ps.append(id);
        txn.exec(lib::Meta::ModelMeta<T>::Query::deleteById, ps);
    }

    template <typename T>
    static std::vector<T> selectAllModels(pqxx::work& txn, const T &) {
        auto res = txn.exec(lib::Meta::ModelMeta<T>::Query::selectAll);

        std::vector<T> models;
        models.reserve(res.size());

        for (const auto& row : res) {
            models.push_back(lib::Meta::ModelMeta<T>::fromRow(row));
        }

        return models;
    }

    template <typename T>
    static void updateModel(pqxx::work& txn, const T &model, const std::string &id) {
        auto values = lib::Meta::ModelMeta<T>::extract(model);
        std::apply([&](auto&&... args) {
            pqxx::params ps;
            (lib::Sql::appendParam(ps, args), ...);
            ps.append(id);
            txn.exec(lib::Meta::ModelMeta<T>::Query::updateById, ps);
        }, values);
    }
};
