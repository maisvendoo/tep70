#include    "tep70.h"

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void TEP70::initFuelSystem(const QString &modules_dir, const QString &custom_cfg_dir)
{
    (void) modules_dir;
    (void) custom_cfg_dir;

    electro_fuel_pump = new ElectricFuelPump();
}
