#include    "tep70bs.h"

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void TEP70BS::initFuelSystem(const QString &modules_dir, const QString &custom_cfg_dir)
{
    (void) modules_dir;
    (void) custom_cfg_dir;

    electro_fuel_pump = new ElectricFuelPump();
}
