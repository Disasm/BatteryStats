#include "logfile.h"
#include <string.h>

LogFile::LogFile()
{
    m_fileName = 0;
    m_file = 0;
}

LogFile::~LogFile()
{
    if (m_file) fclose(m_file);
    if (m_fileName) free(m_fileName);
}

bool LogFile::open(const char *fileName)
{
    m_fileName = strdup(fileName);
    m_file = fopen(fileName, "a");
    if (m_file == 0) return false;
    return true;
}

void LogFile::flush()
{
    if (m_file) fflush(m_file);
}

void LogFile::write(BatteryInfo *batteryInfo, const ProcessList &processList)
{
    if (m_file == 0) return;

    struct timespec ts;
    if (clock_gettime(CLOCK_MONOTONIC, &ts) < 0) return;

    float capacity = batteryInfo->capacity();
    bool isCharging = batteryInfo->isCharging();
    fprintf(m_file, "{\"time\": %llu, \"capacity\": %0.2f, \"isCharging\": %s, \"proc\": ", (unsigned long long)ts.tv_sec, capacity, isCharging?"true":"false");
    writeProcessList(processList);
    fprintf(m_file, "},\n");
    fflush(m_file);
}

void LogFile::rotate()
{
    if (m_file == 0) return;

    long pos = ftell(m_file);
    if (pos < 1000000) return;

    // TODO: rotate
}

void LogFile::writeProcessList(const ProcessList &processList)
{
    fprintf(m_file, "[");
    bool first = true;
    for (auto i = processList.cbegin(); i != processList.cend(); i++)
    {
        if (!first)
        {
            fprintf(m_file, ",");
        }
        else
        {
            first = false;
        }
        writeProcessRecord(*i);
    }
    fprintf(m_file, "]");
}

void LogFile::writeProcessRecord(const ProcessRecord &processRecord)
{
    fprintf(m_file, "[\"%s\",%d,%d]", processRecord.name.c_str(), processRecord.pid, processRecord.time);
}
