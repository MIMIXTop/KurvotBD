#pragma once

#include <boost/pfr.hpp>

#include <QVariant>

#include <DataBaseSourse/SqlRequests/Converters.hpp>
#include <Models.hpp>

namespace lib::util {
    template<typename... Lambdas>
    struct match : Lambdas... {
        using Lambdas::operator()...;
    };

    struct FieldCounter {
        template<typename T>
        std::size_t operator()(T) const {
            return boost::pfr::tuple_size_v<T>;
        }
    };

    template<typename... Lambdas>
    match(Lambdas... lambdas) -> match<Lambdas...>;

    struct ToQVarint {
        template <typename T> struct is_optional : std::false_type {};
        template <typename T> struct is_optional<std::optional<T>> : std::true_type {};

        template<typename T>
        QVariant operator() (T&& value) const {
            using ValueType = std::decay_t<T>;

            if constexpr (is_optional<ValueType>::value) {
                if (!value.has_value()) {
                    return {};
                }
                return operator()(*value);
            } else if constexpr (std::is_same_v<ValueType, std::string>) {
                return QString::fromStdString(value);
            } else if constexpr (std::is_enum_v<ValueType>) {
                auto str_value = Converters::toString(value);
                return QString::fromStdString(str_value);
            } else if constexpr (std::is_same_v<ValueType, std::vector<std::string>>) {
                return QString::fromStdString(Converters::toPgArray(value));
            } else {
                return QVariant::fromValue(value);
            }
        }
    };

    template <typename T>
    QVariant getFieldAsQVariant(T&& m_struct, int index) {
        QVariant result;
        boost::pfr::for_each_field(m_struct, [&](auto&& field, std::size_t i) {
            if (static_cast<int>(i) == index) {
                result = ToQVarint{}(field);
            }
        });
        return result;
    }

    template<typename T>
    struct is_one_of : std::disjunction<
                std::is_same<std::decay_t<T>, int>,
                std::is_same<std::decay_t<T>, std::string>,
                std::is_same<std::decay_t<T>, bool>,
                std::is_same<std::decay_t<T>, double>
            > {
    };

    inline lib::Models::Model createModelFromTableName(std::string_view tableName) {
        if (tableName == "department") {
            return lib::Models::Department{};
        } else if (tableName == "position") {
            return lib::Models::Position{};
        } else if (tableName == "client") {
            return lib::Models::Client{};
        } else if (tableName == "employee") {
            return lib::Models::Employee{};
        } else if (tableName == "softwarelicense") {
            return lib::Models::SoftwareLicense{};
        } else if (tableName == "developerspecialization") {
            return lib::Models::DeveloperSpecialization{};
        } else if (tableName == "testerspecialization") {
            return lib::Models::TesterSpecialization{};
        } else if (tableName == "managercertification") {
            return lib::Models::ManagerCertification{};
        } else if (tableName == "project") {
            return lib::Models::Project{};
        } else if (tableName == "projectphase") {
            return lib::Models::ProjectPhase{};
        } else if (tableName == "projectassignment") {
            return lib::Models::ProjectAssignment{};
        } else if (tableName == "licenseallocation") {
            return lib::Models::LicenseAllocation{};
        } else if (tableName == "cloudresource") {
            return lib::Models::CloudResource{};
        } else if (tableName == "worklog") {
            return lib::Models::WorkLog{};
        } else if (tableName == "bug") {
            return lib::Models::Bug{};
        } else if (tableName == "release") {
            return lib::Models::Release{};
        } else if (tableName == "documentation") {
            return lib::Models::Documentation{};
        } else if (tableName == "projectspecification") {
            return lib::Models::ProjectSpecification{};
        }
        
        // Возвращаем пустую модель по умолчанию
        return lib::Models::Employee{};
    }
}
