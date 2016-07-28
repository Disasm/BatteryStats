#include "batteryinfo.h"
#include "genericbatteryinfo.h"

BatteryInfo::BatteryInfo(RawBatteryInfo *raw)
{
    m_raw = raw;
}

bool BatteryInfo::update()
{
    return m_raw->update();
}

BatteryInfo* BatteryInfo::detect()
{
    RawBatteryInfo* raw = RawBatteryInfo::detect();
    return new GenericBatteryInfo(raw);
}
