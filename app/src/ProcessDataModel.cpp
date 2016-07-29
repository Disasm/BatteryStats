#include "ProcessDataModel.h"

ProcessDataModel::ProcessDataModel()
{
    m_items.append(qMakePair(QString("localhost"), 0.7));
    m_items.append(qMakePair(QString("windowsTrueApp"), 0.1));
    m_items.append(qMakePair(QString("dont_kill_me_please"), 1));
}

int ProcessDataModel::size() const
{
    return m_items.size();
}

QVariant ProcessDataModel::get(int index) const
{
    QVariantMap m;
    m["name"] = m_items[index].first;
    m["load"] = m_items[index].second;
    return m;
}

