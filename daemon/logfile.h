#ifndef LOGFILE_H
#define LOGFILE_H

#include <stdio.h>
#include "batteryinfo.h"
#include "processinfo.h"

class LogFile
{
public:
    LogFile();
    ~LogFile();

    bool open(const char* fileName);
    void flush();

    void write(BatteryInfo* batteryInfo, const ProcessList &processList);

    void rotate();

private:
    void writeProcessList(const ProcessList &processList);
    void writeProcessRecord(const ProcessRecord &processRecord);

private:
    char*   m_fileName;
    FILE*   m_file;
};

#endif // LOGFILE_H
