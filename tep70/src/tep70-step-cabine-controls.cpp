#include    "tep70.h"

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void TEP70::stepCabineControls(double t, double dt)
{
    km[cabine_idx]->setControl(keys);
    km[CAB1]->step(t, dt);
    km[CAB2]->step(t, dt);

    tumbler_field_weak1.setControl(keys);
    tumbler_field_weak1.step(t, dt);

    tumbler_field_weak2.setControl(keys);
    tumbler_field_weak2.step(t, dt);

    tumbler_water_zaluzi.setControl(keys);
    tumbler_water_zaluzi.step(t, dt);

    tumbler_oil_zaluzi.setControl(keys);
    tumbler_oil_zaluzi.step(t, dt);

    brake_force_switch.setControl(keys);
    brake_force_switch.step(t, dt);
}
