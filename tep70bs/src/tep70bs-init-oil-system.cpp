#include    "tep70bs.h"

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void TEP70BS::initOilSystem(const QString &modules_dir, const QString &custom_cfg_dir)
{
    (void) modules_dir;
    (void) custom_cfg_dir;

    electro_oil_pump = new ElectricOilPump();
}
