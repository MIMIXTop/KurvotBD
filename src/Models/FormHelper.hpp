#pragma once
#include <QObject>
#include <QVariantMap>
#include <QVariantList>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <iostream>

#include "DataBase.hpp"
#include "FormMeta.hpp"
#include "util/util.hpp"

class FormHelper : public QObject {
    Q_OBJECT
    Q_PROPERTY(QStringList availableModels READ availableModels NOTIFY availableModelsChanged)
    Q_PROPERTY(QString lastError READ lastError NOTIFY lastErrorChanged)

public:
    explicit FormHelper(QObject *parent = nullptr)
        : QObject(parent)
        , database(std::make_shared<DataBase>("host=127.0.0.1 port=5432 dbname=db user=user password=pass"))
        , m_lastError("")
    {
    }

    QString lastError() const { return m_lastError; }

    QStringList availableModels() const {
        return {
            "Department", "Position", "Client", "SoftwareLicense",
            "Employee", "DeveloperSpecialization", "TesterSpecialization",
            "ManagerCertification", "Project", "ProjectPhase",
            "ProjectAssignment", "LicenseAllocation", "CloudResource",
            "WorkLog", "Bug", "Release", "Documentation", "ProjectSpecification"
        };
    }

signals:
    void availableModelsChanged();
    void lastErrorChanged();

public slots:
    /**
     * Преобразует имя таблицы в имя модели
     */
    QString tableNameToModelName(const QString& tableName) {
        // Словарь соответствий table_name -> ModelName
        static QMap<QString, QString> mapping = {
            {"department", "Department"},
            {"position", "Position"},
            {"client", "Client"},
            {"software_license", "SoftwareLicense"},
            {"softwarelicense", "SoftwareLicense"},
            {"employee", "Employee"},
            {"developer_specialization", "DeveloperSpecialization"},
            {"developerspecialization", "DeveloperSpecialization"},
            {"tester_specialization", "TesterSpecialization"},
            {"testerspecialization", "TesterSpecialization"},
            {"manager_certification", "ManagerCertification"},
            {"managercertification", "ManagerCertification"},
            {"project", "Project"},
            {"project_phase", "ProjectPhase"},
            {"projectphase", "ProjectPhase"},
            {"project_assignment", "ProjectAssignment"},
            {"projectassignment", "ProjectAssignment"},
            {"license_allocation", "LicenseAllocation"},
            {"licenseallocation", "LicenseAllocation"},
            {"cloudresource", "CloudResource"},
            {"cloud_resource", "CloudResource"},
            {"worklog", "WorkLog"},
            {"work_log", "WorkLog"},
            {"bug", "Bug"},
            {"release", "Release"},
            {"documentation", "Documentation"},
            {"project_specification", "ProjectSpecification"},
            {"projectspecification", "ProjectSpecification"}
        };
        
        return mapping.value(tableName, tableName);
    }

    /**
     * Возвращает JSON с конфигурацией полей для модели
     */
    QString getFormConfig(const QString& modelName) {
        auto def = lib::FormMeta::getFormDefinition(modelName.toStdString());
        QJsonArray fields;
        for (const auto& field : def.fields) {
            QJsonObject obj;
            obj["name"] = QString::fromStdString(field.name);
            obj["display_name"] = QString::fromStdString(field.display_name);
            obj["type"] = fieldTypeToString(field.type);
            obj["is_primary_key"] = field.is_primary_key;
            obj["is_auto_generated"] = field.is_auto_generated;

            // Enum options
            if (!field.enum_options.empty()) {
                QJsonArray options;
                for (const auto& opt : field.enum_options) {
                    QJsonObject optObj;
                    optObj["value"] = QString::fromStdString(opt.value);
                    optObj["display_name"] = QString::fromStdString(opt.display_name);
                    options.append(optObj);
                }
                obj["enum_options"] = options;
            }

            // FK info
            if (field.fk_info.has_value()) {
                QJsonObject fkObj;
                fkObj["source_table"] = QString::fromStdString(field.fk_info->source_table);
                fkObj["id_column"] = QString::fromStdString(field.fk_info->id_column);
                fkObj["display_column"] = QString::fromStdString(field.fk_info->display_column);
                obj["fk_info"] = fkObj;
            }

            fields.append(obj);
        }

        QJsonObject root;
        root["model_name"] = QString::fromStdString(def.model_name);
        root["table_name"] = QString::fromStdString(def.table_name);
        root["fields"] = fields;

        return QString(QJsonDocument(root).toJson(QJsonDocument::Compact));
    }

    /**
     * Возвращает данные для ComboBox по внешнему ключу
     * Формат: [{"value": 1, "display": "Name"}, ...]
     */
    QVariantList getForeignKeyData(const QString& sourceTable) {
        QVariantList result;
        try {
            std::string sql;
            std::string idCol;
            std::string displayCol;

            if (sourceTable == "department") {
                sql = "SELECT department_id, name FROM department ORDER BY name";
                idCol = "department_id";
                displayCol = "name";
            } else if (sourceTable == "position") {
                sql = "SELECT position_id, title FROM position ORDER BY title";
                idCol = "position_id";
                displayCol = "title";
            } else if (sourceTable == "client") {
                sql = "SELECT client_id, name FROM client ORDER BY name";
                idCol = "client_id";
                displayCol = "name";
            } else if (sourceTable == "employee") {
                sql = "SELECT employee_id, last_name FROM employee ORDER BY last_name";
                idCol = "employee_id";
                displayCol = "last_name";
            } else if (sourceTable == "project") {
                sql = "SELECT project_id, name FROM project ORDER BY name";
                idCol = "project_id";
                displayCol = "name";
            } else if (sourceTable == "software_license" || sourceTable == "softwarelicense") {
                sql = "SELECT license_id, name FROM softwarelicense ORDER BY name";
                idCol = "license_id";
                displayCol = "name";
            } else if (sourceTable == "project_phase") {
                sql = "SELECT phase_id, name FROM project_phase ORDER BY order_number";
                idCol = "phase_id";
                displayCol = "name";
            } else if (sourceTable == "project_phase") {
                sql = "SELECT phase_id, name FROM project_phase ORDER BY order_number";
                idCol = "phase_id";
                displayCol = "name";
            } else {
                return result;
            }

            pqxx::work txn(database->getConnection());
            auto res = txn.exec(sql);

            for (const auto& row : res) {
                QVariantMap item;
                item["value"] = row[idCol.c_str()].as<int>();
                item["display"] = QString::fromStdString(row[displayCol.c_str()].as<std::string>());
                result.append(item);
            }

            txn.commit();
        } catch (const std::exception& e) {
            std::cerr << "Error getting FK data: " << e.what() << std::endl;
        }

        return result;
    }

    /**
     * Вставка новой записи
     */
    bool insertRecord(const QString& modelName, const QVariantMap& data) {
        try {
            auto def = lib::FormMeta::getFormDefinition(modelName.toStdString());
            if (def.table_name.empty()) {
                std::cerr << "Unknown model: " << modelName.toStdString() << std::endl;
                return false;
            }

            // Построение SQL запроса
            std::vector<std::string> columns;
            std::vector<std::string> placeholders;
            std::vector<std::string> values;
            
            int paramIndex = 1;
            for (const auto& field : def.fields) {
                // Пропускаем автогенерируемые поля и первичные ключи
                if (field.is_auto_generated || field.is_primary_key) {
                    continue;
                }
                
                QString fieldName = QString::fromStdString(field.name);
                if (!data.contains(fieldName)) {
                    continue;
                }
                
                columns.push_back(field.name);
                placeholders.push_back("$" + std::to_string(paramIndex++));
                
                // Преобразование значения в строку для SQL
                QVariant value = data[fieldName];
                if (value.isNull() || !value.isValid()) {
                    values.push_back("NULL");
                } else if (value.type() == QVariant::Bool) {
                    values.push_back(value.toBool() ? "true" : "false");
                } else if (value.type() == QVariant::Int || value.type() == QVariant::Double) {
                    values.push_back(value.toString().toStdString());
                } else if (value.type() == QVariant::StringList) {
                    // Массив строк для PostgreSQL
                    QStringList list = value.toStringList();
                    std::string arrayStr = "ARRAY[";
                    for (int i = 0; i < list.size(); ++i) {
                        if (i > 0) arrayStr += ",";
                        arrayStr += "'" + list[i].toStdString() + "'";
                    }
                    arrayStr += "]";
                    values.push_back(arrayStr);
                } else {
                    // Проверка на пустую строку - для дат и FK это должно быть NULL
                    QString strValue = value.toString();
                    if (strValue.trimmed().isEmpty()) {
                        values.push_back("NULL");
                    } else {
                        values.push_back("'" + strValue.toStdString() + "'");
                    }
                }
            }
            
            if (columns.empty()) {
                std::cerr << "No fields to insert" << std::endl;
                return false;
            }
            
            // Формирование SQL запроса
            std::string sql = "INSERT INTO " + def.table_name + " (";
            for (size_t i = 0; i < columns.size(); ++i) {
                if (i > 0) sql += ", ";
                sql += columns[i];
            }
            sql += ") VALUES (";
            for (size_t i = 0; i < values.size(); ++i) {
                if (i > 0) sql += ", ";
                sql += values[i];
            }
            sql += ")";
            
            std::cout << "INSERT SQL: " << sql << std::endl;
            
            pqxx::work txn(database->getConnection());
            txn.exec(sql);
            txn.commit();
            
            m_lastError = "";
            emit lastErrorChanged();
            return true;
        } catch (const std::exception& e) {
            std::cerr << "Error inserting: " << e.what() << std::endl;
            m_lastError = QString::fromStdString(e.what());
            emit lastErrorChanged();
            return false;
        }
    }

    /**
     * Обновление записи по ID
     */
    bool updateRecord(const QString& modelName, int id, const QVariantMap& data) {
        try {
            auto def = lib::FormMeta::getFormDefinition(modelName.toStdString());
            if (def.table_name.empty()) {
                std::cerr << "Unknown model: " << modelName.toStdString() << std::endl;
                return false;
            }

            // Построение SQL запроса UPDATE
            std::vector<std::string> setPairs;
            
            for (const auto& field : def.fields) {
                // Пропускаем автогенерируемые поля и первичные ключи
                if (field.is_auto_generated || field.is_primary_key) {
                    continue;
                }
                
                QString fieldName = QString::fromStdString(field.name);
                if (!data.contains(fieldName)) {
                    continue;
                }
                
                // Преобразование значения в строку для SQL
                QVariant value = data[fieldName];
                std::string valueStr;
                
                if (value.isNull() || !value.isValid()) {
                    valueStr = "NULL";
                } else if (value.type() == QVariant::Bool) {
                    valueStr = value.toBool() ? "true" : "false";
                } else if (value.type() == QVariant::Int || value.type() == QVariant::Double) {
                    valueStr = value.toString().toStdString();
                } else if (value.type() == QVariant::StringList) {
                    // Массив строк для PostgreSQL
                    QStringList list = value.toStringList();
                    valueStr = "ARRAY[";
                    for (int i = 0; i < list.size(); ++i) {
                        if (i > 0) valueStr += ",";
                        valueStr += "'" + list[i].toStdString() + "'";
                    }
                    valueStr += "]";
                } else {
                    // Проверка на пустую строку - для дат и FK это должно быть NULL
                    QString strValue = value.toString();
                    if (strValue.trimmed().isEmpty()) {
                        valueStr = "NULL";
                    } else {
                        valueStr = "'" + strValue.toStdString() + "'";
                    }
                }
                
                setPairs.push_back(field.name + " = " + valueStr);
            }
            
            if (setPairs.empty()) {
                std::cerr << "No fields to update" << std::endl;
                return false;
            }
            
            // Найти имя первичного ключа
            std::string pkName;
            for (const auto& field : def.fields) {
                if (field.is_primary_key) {
                    pkName = field.name;
                    break;
                }
            }
            
            if (pkName.empty()) {
                std::cerr << "Primary key not found for model: " << modelName.toStdString() << std::endl;
                return false;
            }
            
            // Формирование SQL запроса
            std::string sql = "UPDATE " + def.table_name + " SET ";
            for (size_t i = 0; i < setPairs.size(); ++i) {
                if (i > 0) sql += ", ";
                sql += setPairs[i];
            }
            sql += " WHERE " + pkName + " = " + std::to_string(id);
            
            std::cout << "UPDATE SQL: " << sql << std::endl;
            
            pqxx::work txn(database->getConnection());
            txn.exec(sql);
            txn.commit();
            
            m_lastError = "";
            emit lastErrorChanged();
            return true;
        } catch (const std::exception& e) {
            std::cerr << "Error updating: " << e.what() << std::endl;
            m_lastError = QString::fromStdString(e.what());
            emit lastErrorChanged();
            return false;
        }
    }

private:
    std::shared_ptr<DataBase> database;
    QString m_lastError;

    QString fieldTypeToString(lib::FormMeta::FieldType type) {
        switch (type) {
            case lib::FormMeta::FieldType::Text: return "Text";
            case lib::FormMeta::FieldType::TextArea: return "TextArea";
            case lib::FormMeta::FieldType::Date: return "Date";
            case lib::FormMeta::FieldType::OptionalDate: return "OptionalDate";
            case lib::FormMeta::FieldType::Number: return "Number";
            case lib::FormMeta::FieldType::DoubleNumber: return "DoubleNumber";
            case lib::FormMeta::FieldType::Boolean: return "Boolean";
            case lib::FormMeta::FieldType::Enum: return "Enum";
            case lib::FormMeta::FieldType::ForeignKey: return "ForeignKey";
            case lib::FormMeta::FieldType::OptionalForeignKey: return "OptionalForeignKey";
            case lib::FormMeta::FieldType::StringArray: return "StringArray";
        }
        return "Text";
    }
};
