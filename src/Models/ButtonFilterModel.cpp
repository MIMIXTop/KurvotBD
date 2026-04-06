#include "ButtonFilterModel.hpp"

ButtonFilterModel::ButtonFilterModel(QObject* parent)
    : QAbstractListModel(parent)
{
    for (const auto& [type, label] : filterLabels) {
        m_filters.push_back(type);
    }
}

int ButtonFilterModel::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid()) return 0;
    return static_cast<int>(m_filters.size());
}

QVariant ButtonFilterModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || index.row() >= static_cast<int>(m_filters.size()))
        return {};

    const auto filterType = m_filters[index.row()];

    switch (role) {
        case NameRole: {
            for (const auto& [type, label] : filterLabels) {
                if (type == filterType) {
                    return QString::fromUtf8(label.data(), static_cast<int>(label.size()));
                }
            }
            return {};
        }
        case IdRole:
            return static_cast<int>(filterType);
        default:
            return {};
    }
}

QHash<int, QByteArray> ButtonFilterModel::roleNames() const
{
    return {
        {NameRole, "name"},
        {IdRole, "filterType"}
    };
}

void ButtonFilterModel::selectFilter(int index)
{
    if (index < 0 || index >= static_cast<int>(m_filters.size()))
        return;
    emit filterSelected(static_cast<int>(m_filters[index]));
}
