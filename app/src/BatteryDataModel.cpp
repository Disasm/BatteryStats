#include "BatteryDataModel.h"

BatteryDataModel::BatteryDataModel()
{
    m_elapsed = 0;
    m_logFile = 0;
    for (int i = 0; i < 100; i++)
    {
        Item item;
        item.capacity = i / 100.f;
        item.isCharging = i > 50;
        item.time = i;
        m_elapsed += item.time;
        m_items.append(item);
    }
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
    sizeChanged();
}
