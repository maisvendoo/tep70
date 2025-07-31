#include    "tep70.h"

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void TEP70::stepCabineControls(double t, double dt)
{
    km[cabine_idx]->setControl(keys);
    km[CAB1]->step(t, dt);
    km[CAB2]->step(t, dt);

    tumbler_field_weak1[cabine_idx].setControl(keys);
    tumbler_field_weak1[CAB1].step(t, dt);
    tumbler_field_weak1[CAB2].step(t, dt);

    tumbler_field_weak2[cabine_idx].setControl(keys);
    tumbler_field_weak2[CAB1].step(t, dt);
    tumbler_field_weak2[CAB2].step(t, dt);

    tumbler_water_zaluzi[cabine_idx].setControl(keys);
    tumbler_water_zaluzi[CAB1].step(t, dt);
    tumbler_water_zaluzi[CAB2].step(t, dt);

    tumbler_oil_zaluzi[cabine_idx].setControl(keys);
    tumbler_oil_zaluzi[CAB1].step(t, dt);
    tumbler_oil_zaluzi[CAB2].step(t, dt);

    brake_force_switch.setControl(keys);
    brake_force_switch.step(t, dt);
}
