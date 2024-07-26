#include    "tep70bs.h"

#include    <QDir>

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void TEP70BS::initCabineControls(const QString &modules_dir, const QString &custom_cfg_dir)
{
    (void) modules_dir;

    km = new ControllerKM2202();
    km->read_config("km-2202", custom_cfg_dir);

    tumbler_field_weak1.setKolStates(3);
    tumbler_field_weak1.setKeyCode(KEY_3);
    tumbler_field_weak1.setState(1);

    tumbler_field_weak2.setKolStates(3);
    tumbler_field_weak2.setKeyCode(KEY_4);
    tumbler_field_weak2.setState(1);

    tumbler_water_zaluzi.setKolStates(3);
    tumbler_water_zaluzi.setKeyCode(KEY_5);
    tumbler_water_zaluzi.setState(1);

    tumbler_oil_zaluzi.setKolStates(3);
    tumbler_oil_zaluzi.setKeyCode(KEY_6);
    tumbler_oil_zaluzi.setState(1);

    tumbler_revers.setKolStates(3);
    tumbler_revers.setKeyCode(KEY_W);
    tumbler_revers.setState(1);
}
