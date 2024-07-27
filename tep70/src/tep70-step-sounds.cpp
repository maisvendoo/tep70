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

    // Устройство блокировки тормозов
    analogSignal[SOUND_BRAKE_LOCK_CHANGE_LOCK_POS] = brake_lock->getSoundSignal(BrakeLock::CHANGE_LOCK_POS_SOUND);
    analogSignal[SOUND_BRAKE_LOCK_CHANGE_COMB_POS] = brake_lock->getSoundSignal(BrakeLock::CHANGE_COMB_POS_SOUND);
    analogSignal[SOUND_BRAKE_LOCK_BP_DRAIN_FLOW] = brake_lock->getSoundSignal(BrakeLock::BP_DRAIN_FLOW_SOUND);
    // Кран поездного тормоза
    analogSignal[SOUND_BRAKE_CRANE_CHANGE_POS] = brake_crane->getSoundSignal(BrakeCrane::CHANGE_POS_SOUND);
    analogSignal[SOUND_BRAKE_CRANE_ER_STAB_FLOW] = brake_crane->getSoundSignal(BrakeCrane::ER_STAB_SOUND);
    analogSignal[SOUND_BRAKE_CRANE_ER_FILL_FLOW] = brake_crane->getSoundSignal(BrakeCrane::ER_FILL_FLOW_SOUND);
    analogSignal[SOUND_BRAKE_CRANE_ER_DRAIN_FLOW] = brake_crane->getSoundSignal(BrakeCrane::ER_DRAIN_FLOW_SOUND);
    analogSignal[SOUND_BRAKE_CRANE_BP_FILL_FLOW] = brake_crane->getSoundSignal(BrakeCrane::BP_FILL_FLOW_SOUND);
    analogSignal[SOUND_BRAKE_CRANE_BP_DRAIN_FLOW] = brake_crane->getSoundSignal(BrakeCrane::BP_DRAIN_FLOW_SOUND);
    // Кран локомотивного тормоза
    analogSignal[SOUND_LOCO_CRANE_CHANGE_POS] = loco_crane->getSoundSignal(LocoCrane::CHANGE_POS_SOUND);
    analogSignal[SOUND_LOCO_CRANE_BC_FILL_FLOW] = loco_crane->getSoundSignal(LocoCrane::BC_FILL_FLOW_SOUND);
    analogSignal[SOUND_LOCO_CRANE_BC_DRAIN_FLOW] = loco_crane->getSoundSignal(LocoCrane::BC_DRAIN_FLOW_SOUND);

    // Свистулька и тифулька :-)
    analogSignal[SOUND_SVISTOK] = horn->getSoundSignal(TrainHorn::SVISTOK_SOUND);
    analogSignal[SOUND_TIFON] = horn->getSoundSignal(TrainHorn::TIFON_SOUND);
}
