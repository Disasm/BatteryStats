#include "processinfo.h"
#include <algorithm>
//#include <proc/readproc.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

bool dividerValid = false;
float divider = 1.0;

/*float convertTime(unsigned long long tm)
{
    if (!dividerValid)
    {
        divider = sysconf(_SC_CLK_TCK);
        dividerValid = true;
    }
    return tm / divider;
}*/

bool parseProcessRecord(const std::string &str, ProcessRecord &r)
{
    std::string s = str;

    size_t p = s.find(' ');
    if (p == std::string::npos) return false;

    std::string s_pid = s.substr(0, p);
    s = s.substr(p+1, 0);

    p = s.find(' ');
    if (p == std::string::npos) return false;

    std::string s_time = s.substr(0, p);
    s = s.substr(p+1, 0);

    p = s.find(' ');
    if (p != std::string::npos)
    {
        s = s.substr(0, p);
    }

    std::string s_name = s;
    p = s_name.rfind('/');
    if (p != std::string::npos)
    {
        s_name = s_name.substr(p+1);
    }

    float time = 0.0f;
    p = s_time.find(':');
    time += std::atoi(s_time.substr(0, p).c_str());
    s_time.substr(p+1);
    time *= 60;
    p = s_time.find(':');
    time += std::atoi(s_time.substr(0, p).c_str());
    s_time.substr(p+1);
    time *= 60;
    time += std::atoi(s_time.c_str());

    r.pid = std::atoi(s_pid.c_str());
    r.time = time;
    r.name = s_name;
    return true;
}

ProcessList getProcessInfo()
{
    ProcessList result;

    int pp[2];
    if(pipe(pp) == -1) return result;

    pid_t ch_pid;
    if((ch_pid = fork()) == 0)
    {
        close(1);
        close(pp[0]);
        dup2(pp[1],1);
        execlp("ps","ps", "axo", "pid,time,args", NULL);
        return result;
    }
    else
    {
        close(pp[1]);
        waitpid(ch_pid,NULL,0);

        std::string s;

        char buf[1024];
        while (true)
        {
            ssize_t len = read(pp[0], buf, sizeof(buf)-1);
            if (len <= 0) break;
            buf[len] = 0;

            s += std::string(buf);
        }
        close(pp[0]);

        while (s.length() > 0)
        {
            size_t p = s.find('\n');
            std::string line;
            if (p == std::string::npos)
            {
                line = s;
                s.clear();
            }
            else
            {
                line = s.substr(0, p);
                s = s.substr(p+1);
            }

            while (line.length() > 0 && line[0] == ' ') line = line.substr(1);
            if (line.length() == 0) continue;
            if (line.substr(0, 3) == "PID") continue;

            //printf("%s\n", line.c_str());
            ProcessRecord r;
            if (parseProcessRecord(line, r))
            {
                result.push_back(r);
            }
        }
    }
    return result;
}


/*ProcessList getProcessInfo()
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
*/
