#include "driverlib.h"
#include "hal.h"
#include "app_bist.h"
#include "bsp_console.h"

int main(void) {

    //Stop watch dog and check board id.
    WDT_A_hold(WDT_A_BASE);

    switch(Hal_Mcu_getBoardId())
    {
    case 0:
    {
        Hal_Mcu_init();
        App_Bist();
        break;
    }
    default:
    {
        Hal_Mcu_init();
        break;
    }
    }

    //Turn on watch dog and enter normal operation.
    WDT_A_hold(WDT_A_BASE);
    while(1)
    {
        Bsp_Console_Uart();

        _NOP();
    }
}
