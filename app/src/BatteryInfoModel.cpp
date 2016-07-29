#include "BatteryInfoModel.h"

BatteryInfoModel::BatteryInfoModel()
{
    m_logFile = 0;
    Info info;
    info.batteryCapacity = "22000000";
    info.batteryCharge = "0x00000080";
    info.batteryWatts = "0x00000F49";
    m_info.append(info);
}

int BatteryInfoModel::size() const
{
    return 1;
}

QVariant BatteryInfoModel::get(int index) const
{
    QVariantMap m;
    m["capacity"] = m_info.at(index).batteryCapacity;
    m["charge"] = m_info.at(index).batteryCharge;
    m["watts"] = m_info.at(index).batteryWatts;
    return m;
}

void BatteryInfoModel::setLogFile(LogFile *logFile)
{
    qWarning("setLogFile");
    m_logFile = logFile;
    connect(m_logFile, SIGNAL(updated()), this, SLOT(logFileChanged()));
}

void BatteryInfoModel::forceUpdate()
{
    qWarning("forceUpdate");
    if (m_logFile) m_logFile->update();
}

void BatteryInfoModel::logFileChanged()
{
    qWarning("LogFile changed");
    if (m_logFile == 0) return;
}
