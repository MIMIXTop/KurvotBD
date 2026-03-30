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
};
