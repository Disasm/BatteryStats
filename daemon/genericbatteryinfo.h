#ifndef GENERICBATTERYINFO_H
#define GENERICBATTERYINFO_H

#include "batteryinfo.h"

class GenericBatteryInfo : public BatteryInfo
{
    enum Source
    {
        INVALID,
        ENERGY,     // uWh
        CHARGE,     // uAh
        CAPACITY,   // percents
    };

public:
    GenericBatteryInfo(RawBatteryInfo* raw);

    /* Implementation of BatteryInfo.  */
    float capacity();

    /* Implementation of BatteryInfo.  */
    bool isCharging();

private:
    /* Detect valid information source.  */
    void probe();
    bool probeEnergy();
    bool probeCharge();

private:
    Source  m_source;
};

#endif // GENERICBATTERYINFO_H
