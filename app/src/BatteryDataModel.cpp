#include "BatteryDataModel.h"

BatteryDataModel::BatteryDataModel()
{
    float prev = 0.0;
    float dir = 1.0;
    while (m_items.size() < 100)
    {
        int rlen = (((float)qrand()) / RAND_MAX) * 30 + 10;
        for (int i = 0; i < rlen; i++)
        {
            if (m_items.size() > 100)
            {
                break;
            }

            float r = ((float)qrand()) / (RAND_MAX * 20);
            float c = prev + r * dir;
            if (c < 0) c = 0.0;
            if (c > 1.0) c = 1.0;

            bool isCharging = dir > 0;
            QPair<float, bool> m = qMakePair(c, isCharging);
            m_items.append(m);

            prev = c;
        }
        dir = -dir;
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

