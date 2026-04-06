#pragma once

#include <QAbstractListModel>
#include <string_view>
#include <vector>
#include <array>

class ButtonFilterModel : public QAbstractListModel {
    Q_OBJECT

public:
    enum class FilterType : uint8_t {
        employees,
        projects,
        testing,
        finance,
        reports
    };
    Q_ENUM(FilterType)

    enum ButtonFilterModelRoles {
        NameRole = Qt::UserRole + 1,
        IdRole
    };

    static constexpr std::array<std::pair<FilterType, std::string_view>, 5> filterLabels = {{
        {FilterType::employees, "👥 Сотрудники и Кадры"},
        {FilterType::projects, "📁 Проекты и Заказчики"},
        {FilterType::testing, "🐞 Тестирование и Баги"},
        {FilterType::finance, "💰 Финансы и Инфраструктура"},
        {FilterType::reports, "📊 Отчеты и Документы"}
    }};

    explicit ButtonFilterModel(QObject* parent = nullptr);

    int rowCount(const QModelIndex& parent = {}) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

    Q_INVOKABLE void selectFilter(int index);

signals:
    void filterSelected(int filterType);

protected:
    QHash<int, QByteArray> roleNames() const override;

private:
    std::vector<FilterType> m_filters;
};
