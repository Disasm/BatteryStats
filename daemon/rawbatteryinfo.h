#ifndef RAWBATTERYINFO_H
#define RAWBATTERYINFO_H

#include <string>
#include <map>

class RawBatteryInfo
{
public:
    RawBatteryInfo(const std::string &path);

    bool update();

    std::string getString(const char *name) const;
    int getInt(const char *name, int def) const;

    void dump();

    static RawBatteryInfo* detect();

private:
    std::string m_path;
    std::map<std::string, std::string> m_values;
};

#endif // RAWBATTERYINFO_H
