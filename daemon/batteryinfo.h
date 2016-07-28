#ifndef BATTERYINFO_H
#define BATTERYINFO_H

#include "rawbatteryinfo.h"

class BatteryInfo
{
public:
    BatteryInfo(RawBatteryInfo* raw);

    bool update();

    /* Get capacity in [0;1].  */
    virtual float capacity() = 0;

    virtual bool isCharging() = 0;

    static BatteryInfo* detect();

protected:
    RawBatteryInfo* m_raw;
};

#endif // BATTERYINFO_H
