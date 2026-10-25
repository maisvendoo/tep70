#include    "tep70.h"

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void TEP70::initCooler(const QString &modules_dir, const QString &custom_cfg_dir)
{
    (void) modules_dir;

    fan_hot = new HydrostaticFanDrive();
    fan_hot->read_config("hydrostatic-fan-drive", custom_cfg_dir);

    fan_cold = new HydrostaticFanDrive();
    fan_cold->read_config("hydrostatic-fan-drive", custom_cfg_dir);

    fan_hot_aero = new CoolerFan();
    fan_hot_aero->read_config("cooler-fan", custom_cfg_dir);

    fan_cold_aero = new CoolerFan();
    fan_cold_aero->read_config("cooler-fan", custom_cfg_dir);

    vzhv = new PneumoElectroValve();
    vzhv->read_config("pneumo-electro-valve", custom_cfg_dir);
    vzhv->setVoltage(0.0);

    vzhm = new PneumoElectroValve();
    vzhm->read_config("pneumo-electro-valve", custom_cfg_dir);
    vzhm->setVoltage(0.0);

    rtv = new HysteresisRelay(63.0, 68.0, false);
    rtm = new HysteresisRelay(60.0, 65.0, false);
}