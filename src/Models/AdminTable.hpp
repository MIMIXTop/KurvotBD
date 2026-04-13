#pragma once
#include <QAbstractTableModel>

#include "DataBase.hpp"
#include "Models.hpp"


class AdminTable : public QAbstractTableModel {
    Q_OBJECT

    Q_PROPERTY(QStringList headerList READ headersList NOTIFY headerListChanged)
    Q_PROPERTY(QStringList tableNameList READ tableNameList NOTIFY tableNameListChanged)
    Q_PROPERTY(QString currentTableName READ currentTableName WRITE setCurrentTableName NOTIFY currentTableNameChanged)

public:
    AdminTable(QObject *parent = nullptr) : QAbstractTableModel(parent), database(std::make_shared<DataBase>("host=127.0.0.1 port=5432 dbname=db user=user password=pass")) {
    }

    void initialize() {
        updateTableNameList();
    }

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    QStringList headersList() const { return m_headerList; }

    QString currentTableName() const { return m_currentTableName; }

    QStringList tableNameList() const { return m_tableNameList; }

    void setCurrentTableName(const QString &currentTableName) {
        if (currentTableName != m_currentTableName) {
            m_currentTableName = currentTableName;
            emit currentTableNameChanged();
            emit headerListChanged();
        }
    }

public slots:
    void loadTableData();

signals:
    void headerListChanged();
    void currentTableNameChanged();
    void tableNameListChanged();

protected:
    QHash<int, QByteArray> roleNames() const override;

private:
    QStringList m_headerList;
    QStringList m_tableNameList;
    QString m_currentTableName;

    std::vector<lib::Models::Model> m_models;
    std::shared_ptr<DataBase> database;

    void updateHeaderList();
    void updateTableNameList();
};
