#include "BatteryDataModel.h"

BatteryDataModel::BatteryDataModel()
{
    for (int i = 0; i < 100; i++)
    {
        QPair<float, bool> v = qMakePair(i / 100.0f, true);
        m_items.append(v);
    }
}

int BatteryDataModel::size() const
{
    return m_items.size();
}

QVariant BatteryDataModel::get(int index) const
{
    QVariantMap m;
    m["capacity"] = m_items[index].first;
    m["charging"] = m_items[index].second;
    return m;
}

