#include "processinfo.h"
#include <proc/readproc.h>
#include <string.h>

bool dividerValid = false;
float divider = 1.0;

float convertTime(unsigned long long tm)
{
    if (!dividerValid)
    {
        divider = sysconf(_SC_CLK_TCK);
        dividerValid = true;
    }
    return tm / divider;
}

ProcessList getProcessInfo()
{
    ProcessList result;

    proc_t** proctab = readproctab(PROC_FILLMEM | PROC_FILLSTAT | PROC_FILLSTATUS | PROC_FILLCOM);
    proc_t** head = proctab;

    unsigned long long systemTime = 0;
    while (*head)
    {
        proc_t* p = *head;

        std::string name;
        if (p->cmdline)
        {
            name = std::string(*(p->cmdline));
            size_t p = name.find(' ');
            if (p != std::string::npos)
            {
                name = name.substr(0, p);
            }

            p = name.rfind('/');
            if (p != std::string::npos)
            {
                name = name.substr(p+1);
            }
        }
        if (!name.empty())
        {
            ProcessRecord r;
            r.pid = p->tid;
            r.time = convertTime(p->utime + p->stime);
            r.name = name;

            result.push_back(r);
        }
        else
        {
            systemTime += p->utime + p->stime;
        }

        freeproc(p);
        head++;
    }

    ProcessRecord r;
    r.pid = -1;
    r.time = convertTime(systemTime);
    r.name = "<system>";
    result.push_back(r);

    return result;
}
