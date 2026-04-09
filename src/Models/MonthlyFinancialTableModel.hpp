#pragma once

#include <QAbstractTableModel>

#include "lib/src/SqlFunctions.hpp"
#include "lib/src/DataBase.hpp"

class MonthlyFinancialTableModel : public QAbstractTableModel {
    Q_OBJECT

public:
    enum MonthlyFinancialRoles {
        ProjectName = Qt::UserRole + 1,
        ExpensesSalary,
        ExpensesLicenses,
        ExpensesCloud,
        TotalMonthlyCost
    };
    Q_ENUM(MonthlyFinancialRoles)

    explicit MonthlyFinancialTableModel(QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    Q_INVOKABLE void loadReport(int month, int year);

protected:
    QHash<int, QByteArray> roleNames() const override;

private:
    std::vector<lib::SqlFunctions::MonthlyFinancialReportResult> financialReport;
    std::shared_ptr<DataBase> database;
};
