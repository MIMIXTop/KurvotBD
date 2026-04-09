#include "MonthlyFinancialTableModel.hpp"
#include <future>

MonthlyFinancialTableModel::MonthlyFinancialTableModel(QObject *parent)
    : QAbstractTableModel(parent)
    , database(std::make_shared<DataBase>("host=127.0.0.1 port=5432 dbname=db user=user password=pass"))
{
}

int MonthlyFinancialTableModel::rowCount(const QModelIndex &parent) const {
    if (parent.isValid())
        return 0;
    return static_cast<int>(financialReport.size());
}

int MonthlyFinancialTableModel::columnCount(const QModelIndex &parent) const {
    if (parent.isValid())
        return 0;
    return 5; // ProjectName, ExpensesSalary, ExpensesLicenses, ExpensesCloud, TotalMonthlyCost
}

QVariant MonthlyFinancialTableModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || index.row() >= static_cast<int>(financialReport.size()))
        return {};

    const auto &item = financialReport[index.row()];

    if (role == Qt::DisplayRole) {
        switch (index.column()) {
            case 0: return QString::fromStdString(item.project_name);
            case 1: return item.expenses_salary;
            case 2: return item.expenses_licenses;
            case 3: return item.expenses_cloud;
            case 4: return item.total_monthly_cost;
            default: return {};
        }
    }

    if (role >= ProjectName) {
        switch (role) {
            case ProjectName: return QString::fromStdString(item.project_name);
            case ExpensesSalary: return item.expenses_salary;
            case ExpensesLicenses: return item.expenses_licenses;
            case ExpensesCloud: return item.expenses_cloud;
            case TotalMonthlyCost: return item.total_monthly_cost;
            default: return {};
        }
    }

    return {};
}

void MonthlyFinancialTableModel::loadReport(int month, int year) {
    auto res = std::async([this, month, year] {
        return database->getMonthlyFinancialReport(month, year);
    });

    beginResetModel();
    financialReport = res.get();
    endResetModel();
}

QHash<int, QByteArray> MonthlyFinancialTableModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[ProjectName] = "projectName";
    roles[ExpensesSalary] = "expensesSalary";
    roles[ExpensesLicenses] = "expensesLicenses";
    roles[ExpensesCloud] = "expensesCloud";
    roles[TotalMonthlyCost] = "totalMonthlyCost";
    return roles;
}
