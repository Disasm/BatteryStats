#ifndef PROCESSINFO_H
#define PROCESSINFO_H

#include <string>
#include <list>

typedef struct {
    std::string name;
    int pid;
    int time;
} ProcessRecord;
typedef std::list<ProcessRecord> ProcessList;

ProcessList getProcessInfo();

#endif // PROCESSINFO_H
