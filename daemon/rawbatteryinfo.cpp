#include "rawbatteryinfo.h"
#include <map>
#include <list>
#include <algorithm>
#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>

RawBatteryInfo::RawBatteryInfo(const std::string &path)
{
    m_path = path;
}

bool RawBatteryInfo::update()
{
    m_values.clear();

    FILE* f = fopen(m_path.c_str(), "r");
    if (f == 0) return false;

    char buf[4096];
    size_t r = fread(buf, 1, sizeof(buf), f);
    if (r == 0)
    {
        fclose(f);
        return false;
    }

    buf[r] = 0;
    std::string contents(buf);

    std::list<std::string> lines;
    while (true)
    {
        size_t pos = contents.find('\n');
        if (pos == std::string::npos)
        {
            if (contents.length() > 0)
            {
                lines.push_back(contents);
            }
            break;
        }
        else
        {
            lines.push_back(contents.substr(0, pos));
            contents = contents.substr(pos + 1);
        }
    }

    for (auto i = lines.cbegin(); i != lines.cend(); i++)
    {
        const std::string &s = *i;
        size_t pos = s.find('=');
        if (pos == std::string::npos) continue;

        std::string key = s.substr(0, pos);
        std::string value = s.substr(pos + 1);

        m_values[key] = value;
    }

    return true;
}

std::string RawBatteryInfo::getString(const char* name) const
{
    std::string fullName = name;
    std::transform(fullName.begin(), fullName.end(),fullName.begin(), ::toupper);
    fullName = "POWER_SUPPLY_" + fullName;

    auto i = m_values.find(fullName);
    if (i != m_values.end())
    {
        return i->second;
    }
    else
    {
        return std::string();
    }
}

int RawBatteryInfo::getInt(const char* name, int def) const
{
    std::string value = getString(name);
    if (value.empty()) return def;

    return std::atoi(value.c_str());
}

void RawBatteryInfo::dump()
{
    printf("Variable dump:\n");
    for (auto i = m_values.cbegin(); i != m_values.cend(); i++)
    {
        printf("'%s' = '%s'\n", i->first.c_str(), i->second.c_str());
    }
}

RawBatteryInfo* RawBatteryInfo::detect()
{
    std::string baseDir = "/sys/class/power_supply";
    DIR* dir = opendir(baseDir.c_str());
    if (dir == 0) return 0;

    std::string targetName;

    struct dirent *ent;
    while ((ent = readdir (dir)) != 0)
    {
        std::string name(ent->d_name);
        if (name == "battery")
        {
            targetName = name;
            break;
        }
        if (name.substr(0, 3) == "BAT")
        {
            targetName = name;
            break;
        }
    }
    closedir(dir);

    if (!targetName.empty())
    {
        std::string fullPath = baseDir + "/" + targetName + "/uevent";
        return new RawBatteryInfo(fullPath);
    }

    return 0;
}
