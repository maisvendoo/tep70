#include    <tep70.h>

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void TEP70::stepSoundsSignals(double t, double dt)
{
    (void) t;
    (void) dt;

    // Тумблеры и АЗВ на пульте машиниста
    analogSignal[SOUND_TUMBLER_OP1] = tumbler_field_weak1.getSoundSignal(Trigger::CHANGE_SOUND);
    analogSignal[SOUND_TUMBLER_OP2] = tumbler_field_weak2.getSoundSignal(Trigger::CHANGE_SOUND);
    analogSignal[SOUND_TUMBLER_BLINDS_WATER] = tumbler_water_zaluzi.getSoundSignal(Trigger::CHANGE_SOUND);
    analogSignal[SOUND_TUMBLER_BLINDS_OIL] = tumbler_oil_zaluzi.getSoundSignal(Trigger::CHANGE_SOUND);
    analogSignal[SOUND_AZV_COMMON_CONTROL] = azv_common_control.getSoundSignal(Trigger::CHANGE_SOUND);
    analogSignal[SOUND_AZV_LOCO_CONTROL] = azv_upr_tepl.getSoundSignal(Trigger::CHANGE_SOUND);
    analogSignal[SOUND_AZV_FUIL_PUMP] = azv_fuel_pump.getSoundSignal(Trigger::CHANGE_SOUND);
    analogSignal[SOUND_AZV_EPT] = azv_ept_on.getSoundSignal(Trigger::CHANGE_SOUND);
    analogSignal[SOUND_AZV_EDT] = azv_edt_on.getSoundSignal(Trigger::CHANGE_SOUND);
    analogSignal[SOUND_TUMBLER_VOLTAGE] = tumbler_voltage.getSoundSignal(Trigger::CHANGE_SOUND);
    analogSignal[SOUND_TUMBLER_DISEL_STOP] = tumbler_disel_stop.getSoundSignal(Trigger::CHANGE_SOUND);
}
