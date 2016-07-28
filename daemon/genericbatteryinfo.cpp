#include "genericbatteryinfo.h"

GenericBatteryInfo::GenericBatteryInfo(RawBatteryInfo* raw) : BatteryInfo(raw)
{
    probe();
}

float GenericBatteryInfo::capacity()
{
    if (m_source == ENERGY)
    {
        int energyFull = m_raw->getInt("energy_full", -1);
        int energy = m_raw->getInt("energy_now", 0);
        return ((float)energy) / energyFull;
    }

    if (m_source == CAPACITY)
    {
        int capacity = m_raw->getInt("capacity", 0);
        return capacity / 100.0f;
    }

    return 0.0f;
}

bool GenericBatteryInfo::isCharging()
{
    std::string v = m_raw->getString("status");
    if (v == "Charging") return true;
    if (v == "Discharging") return false;
    return false;
}

void GenericBatteryInfo::probe()
{
    m_raw->update();

    if (probeEnergy())
    {
        m_source = ENERGY;
        return;
    }
    if (probeCharge())
    {
        m_source = CHARGE;
        return;
    }

    int v = m_raw->getInt("capacity", -1);
    if (v >= 0)
    {
        m_source = CAPACITY;
        return;
    }
    m_source = INVALID;
}

bool GenericBatteryInfo::probeEnergy()
{
    int v = m_raw->getInt("energy_full", -1);
    if (v <= 0) return false;
    if (v > 100000000) return false;

    int v2 = m_raw->getInt("energy_now", -1);
    if (v2 < 0) return false;
    if (v2 > v) return false;

    return true;
}

bool GenericBatteryInfo::probeCharge()
{
    int v = m_raw->getInt("charge_full", -1);
    if (v <= 0) return false;
    if (v > 100000000) return false;

    int v2 = m_raw->getInt("charge_now", -1);
    if (v2 < 0) return false;
    if (v2 > v) return false;

    return true;
}
