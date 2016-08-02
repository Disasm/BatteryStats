#include "BatteryDataModel.h"

BatteryDataModel::BatteryDataModel()
{
    m_elapsed = 0;
    m_logFile = 0;
#if 0
    for (int i = 0; i < 100; i++)
    {
        Item item;

        if (i > 50)
        {
            item.capacity = 0.5 - ((i-50) / 200.f);
        }
        else
        {
            item.capacity = i / 100.f;
        }

        item.isCharging = i <= 50;
        item.time = i;
        m_elapsed += item.time;
        m_items.append(item);
    }
#else
    for (int i = 0; i < 100; i++)
    {
        Item item;

        if (i <= 50)
        {
            item.capacity = 1.0f - (i / 100.f);
        }
        else
        {
            item.capacity = 0.5f + (i-50) / 100.f;
        }

        item.isCharging = i > 50;
        item.time = i;
        m_elapsed += item.time;
        m_items.append(item);
    }
#endif
    m_predictionValid = false;
}

int BatteryDataModel::elapsed() const
{
    return m_elapsed;
}

int BatteryDataModel::size() const
{
    return m_items.size();
}

QVariant BatteryDataModel::get(int index) const
{
    QVariantMap m;
    m["capacity"] = m_items[index].capacity;
    m["charging"] = m_items[index].isCharging;
    m["time"] = m_items[index].time;
    return m;
}

int BatteryDataModel::prediction()
{
    if (m_predictionValid) return m_prediction;
    if (m_items.size() < 2) return -1;

    bool chargeFlag =  m_items.last().isCharging;

    QList<Item> dischargeInterval;
    int dtSum = 0;
    float dcSum = 0;
    foreach (const Item &item, m_items)
    {
        if (item.isCharging == !chargeFlag)
        {
            if (dischargeInterval.size() > 2)
            {
                int dt = dischargeInterval.last().time - dischargeInterval.first().time;
                float dc = dischargeInterval.last().capacity - dischargeInterval.first().capacity;
                if (!chargeFlag) dc = -dc;
                dtSum += dt;
                dcSum += dc;
                dischargeInterval.clear();
            }
        }
        else
        {
            dischargeInterval.append(item);
        }
    }
    if (dischargeInterval.size() > 2)
    {
        int dt = dischargeInterval.last().time - dischargeInterval.first().time;
        float dc = dischargeInterval.last().capacity - dischargeInterval.first().capacity;
        if (!chargeFlag) dc = -dc;
        dtSum += dt;
        dcSum += dc;
        qWarning("dc=%0.2f dt=%d", dc, dt);
    }
    qWarning("dtSum=%d, dcSum=%0.2f, chargeFlag=%d", dtSum, dcSum, chargeFlag?1:0);
    if (dcSum < 0.001) return -1;
    float speed = dtSum / dcSum;
    float needCapacity = chargeFlag?(1.0-m_items.last().capacity):m_items.last().capacity;
    int time = needCapacity * speed;
    m_prediction = time;
    m_predictionValid = true;
    return m_prediction;
}

bool BatteryDataModel::charging()
{
    if (m_items.size()==0) return false;
    return m_items.last().isCharging;
}

void BatteryDataModel::setLogFile(LogFile *logFile)
{
    qWarning("setLogFile");
    m_logFile = logFile;
    connect(m_logFile, SIGNAL(updated()), this, SLOT(logFileChanged()));
}

void BatteryDataModel::forceUpdate()
{
    qWarning("forceUpdate");
    if (m_logFile) m_logFile->update();
}

void BatteryDataModel::logFileChanged()
{
    qWarning("LogFile changed");
    if (m_logFile == 0) return;

    m_items.clear();
    QList<LogFile::LogRecord> records = m_logFile->records();
    foreach(const LogFile::LogRecord &r, records)
    {
        Item item;
        item.capacity = r.capacity;
        item.isCharging = r.isCharging;
        item.time = r.time;

        m_items.append(item);
    }
    m_predictionValid = false;
    sizeChanged();
    predictionChanged();
    chargingChanged();
}
