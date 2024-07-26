#include    "tep70.h"

#include    <QDir>

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void TEP70::initCabineControls(const QString &modules_dir, const QString &custom_cfg_dir)
{
    (void) modules_dir;

    km = new ControllerKM2202();
    km->read_config("km-2202", custom_cfg_dir);

    tumbler_field_weak1.setNumPositions(3);
    tumbler_field_weak1.setKeyCode(KEY_3);
    tumbler_field_weak1.setState(1);

    tumbler_field_weak2.setNumPositions(3);
    tumbler_field_weak2.setKeyCode(KEY_4);
    tumbler_field_weak2.setState(1);

    tumbler_water_zaluzi.setNumPositions(3);
    tumbler_water_zaluzi.setKeyCode(KEY_5);
    tumbler_water_zaluzi.setState(1);

    tumbler_oil_zaluzi.setNumPositions(3);
    tumbler_oil_zaluzi.setKeyCode(KEY_6);
    tumbler_oil_zaluzi.setState(1);
}
