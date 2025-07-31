#include    "tep70.h"

#include    <QDir>

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void TEP70::initCabineControls(const QString &modules_dir, const QString &custom_cfg_dir)
{
    (void) modules_dir;

    for (auto i : {CAB1, CAB2})
    {
        km[i] = new ControllerKM2202();
        km[i]->read_config("km-2202", custom_cfg_dir);

        tumbler_field_weak1[i].setNumPositions(3);
        tumbler_field_weak1[i].setKeyCode(KEY_3);
        tumbler_field_weak1[i].setState(1);

        tumbler_field_weak2[i].setNumPositions(3);
        tumbler_field_weak2[i].setKeyCode(KEY_4);
        tumbler_field_weak2[i].setState(1);

        tumbler_water_zaluzi[i].setNumPositions(3);
        tumbler_water_zaluzi[i].setKeyCode(KEY_5);
        tumbler_water_zaluzi[i].setState(1);

        tumbler_oil_zaluzi[i].setNumPositions(3);
        tumbler_oil_zaluzi[i].setKeyCode(KEY_6);
        tumbler_oil_zaluzi[i].setState(1);
    }

    brake_force_switch.setNumPositions(13);
    brake_force_switch.setKeyCode(KEY_F);
    brake_force_switch.setState(0);
}
