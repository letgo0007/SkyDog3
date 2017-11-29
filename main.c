#include "driverlib.h"
#include "hal.h"
#include "bsp_console.h"

int _system_pre_init(void)
{
    WDTCTL = WDTPW | WDTHOLD;

    return 1;
}

int main(void)
{
    Mcu_init();
    Bsp_Console_init();

    while (1)
    {
        Hal_Watchdog_feed();
        Bsp_Console_run();
    }
}
