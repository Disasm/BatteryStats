#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include "rawbatteryinfo.h"
#include "batteryinfo.h"
#include "processinfo.h"
#include "logfile.h"

BatteryInfo* bi;
LogFile log;
bool busy = false;


void tick()
{
    if (busy) return;
    busy = true;

    printf("Updating...\n");

    bi->update();
    ProcessList lst = getProcessInfo();
    log.write(bi, lst);
    log.flush();

    busy = false;
}

void sighandler(int sig)
{
    switch (sig)
    {
    case SIGTERM:
    case SIGINT:
        log.flush();
        exit(0);
        break;
    case SIGUSR1:
        tick();
        break;
    }
}

int main()
{
    bi = BatteryInfo::detect();
    if (bi == 0) return 1;

    log.open("/tmp/battery.log");

    signal(SIGINT, sighandler);
    signal(SIGTERM, sighandler);
    signal(SIGUSR1, sighandler);

    while (true)
    {
        sleep(1);
        tick();
    }

    return 0;
}
