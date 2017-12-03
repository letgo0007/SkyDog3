#include "driverlib.h"
#include "hal.h"
#include "bsp_console.h"
#include "bsp_i2c_cmd.h"
#include "bsp_io.h"
#include "bsp_db.h"
#include "app_bist.h"

int _system_pre_init(void)
{
    WDTCTL = WDTPW | WDTHOLD;

    return 1;
}

int main(void)
{
    Mcu_init();
    Bsp_Console_init();
    Bsp_I2cCmd_init();
    Bsp_Db_init();

    while (1)
    {
        Hal_Watchdog_feed();
        Bsp_IO_scan();
        Bsp_Console_run();
        Bsp_I2cCmd_run();
    }
}
