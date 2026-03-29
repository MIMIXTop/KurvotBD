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

    ~DataBase();

private:
    pqxx::connection connection;

    template <typename T>
    static void insertModel(pqxx::work& txn, const T& model) {
        auto values = lib::Meta::ModelMeta<T>::extract(model);
        std::apply([&](auto&&... args) {
            pqxx::params ps;
            (lib::Sql::appendParam(ps, args), ...);
            txn.exec(lib::Meta::ModelMeta<T>::query, ps);
        }, values);
    }
};
