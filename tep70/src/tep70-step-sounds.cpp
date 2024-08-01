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

    // Топливный насос
    analogSignal[SOUND_FUEL_PUMP] = electro_fuel_pump->getSoundSignal();

    // Маслопрокачивающий насос
    analogSignal[SOUND_OIL_PUMP] = electro_oil_pump->getSoundSignal();

    // Стартер-генератор
    analogSignal[SOUND_STARTER_GENERATOR] = starter_generator->getSoundSignal();

    // Дизель
    for (size_t i = 0; i <= 15; ++i)
    {
        analogSignal[SOUND_DISEL_X0 + i] = disel->getSoundSignal(i);
    }

    // Реверсивкая рукоятка
    analogSignal[SOUND_REVERS_HANDLE] = km->getSoundSignal(ControllerKM2202::REVERS_SHAFT);
    // Главный вал контроллера
    analogSignal[SOUND_CONTROLLER] = km->getSoundSignal(ControllerKM2202::MAIN_SHAFT);

    // Перестуки
    double Vkmh = abs(velocity) * Physics::kmh;
    analogSignal[SOUND_5_10] = sound_state_t::createSoundSignal((Vkmh > 1.0) && (Vkmh <= 10.0), Vkmh / 5.0);
    analogSignal[SOUND_10_20] = sound_state_t::createSoundSignal((Vkmh > 10.0) && (Vkmh <= 20.0));
    analogSignal[SOUND_20_30] = sound_state_t::createSoundSignal((Vkmh > 20.0) && (Vkmh <= 30.0));
    analogSignal[SOUND_30_40] = sound_state_t::createSoundSignal((Vkmh > 30.0) && (Vkmh <= 40.0));
    analogSignal[SOUND_40_50] = sound_state_t::createSoundSignal((Vkmh > 40.0) && (Vkmh <= 50.0));
    analogSignal[SOUND_50_60] = sound_state_t::createSoundSignal((Vkmh > 50.0) && (Vkmh <= 60.0));
    analogSignal[SOUND_60_70] = sound_state_t::createSoundSignal((Vkmh > 60.0) && (Vkmh <= 70.0));
    analogSignal[SOUND_70_80] = sound_state_t::createSoundSignal((Vkmh > 70.0) && (Vkmh <= 80.0));
    analogSignal[SOUND_80_90] = sound_state_t::createSoundSignal((Vkmh > 80.0) && (Vkmh <= 90.0));
    analogSignal[SOUND_90_100] = sound_state_t::createSoundSignal((Vkmh > 90.0) && (Vkmh <= 100.0));
    analogSignal[SOUND_100_110] = sound_state_t::createSoundSignal((Vkmh > 100.0) && (Vkmh <= 110.0));
    analogSignal[SOUND_110_120] = sound_state_t::createSoundSignal((Vkmh > 110.0) && (Vkmh <= 120.0));
    analogSignal[SOUND_120_130] = sound_state_t::createSoundSignal((Vkmh > 120.0) && (Vkmh <= 130.0));
    analogSignal[SOUND_130_140] = sound_state_t::createSoundSignal((Vkmh > 130.0) && (Vkmh <= 140.0));
    analogSignal[SOUND_140_X] = sound_state_t::createSoundSignal(Vkmh > 140.0);

    // Скоростемер
    analogSignal[SOUND_SKOROSTEMER] = speed_meter->getSoundSignal();

    // Мотор-компрессор
    analogSignal[SOUND_MOTOR_COMPRESSOR] = motor_compressor->getSoundSignal();
    analogSignal[SOUND_AZV_MOTOR_COMPRESSOR] = azv_motor_compressor.getSoundSignal(Trigger::CHANGE_SOUND);

    // Реле и контакторы
    analogSignal[SOUND_KONTACTOR_FUEL_PUMP] = kontaktor_fuel_pump->getSoundSignal(Relay::CHANGE_SOUND);
    analogSignal[SOUND_RU8] = ru8->getSoundSignal(Relay::CHANGE_SOUND);
    analogSignal[SOUND_KONTACTOR_OIL_PUMP] = kontaktor_oil_pump->getSoundSignal(Relay::CHANGE_SOUND);
    analogSignal[SOUND_OIL_PUMP_TIME_RELAY] = oilpump_time_relay->getSoundSignal(TimeRelay::CHANGE_SOUND);
    analogSignal[SOUND_STARTER_TIME_RELAY] = starter_time_relay->getSoundSignal(TimeRelay::CHANGE_SOUND);
    analogSignal[SOUND_RU10] = ru10->getSoundSignal(Relay::CHANGE_SOUND);
    analogSignal[SOUND_RU6] = ru6->getSoundSignal(Relay::CHANGE_SOUND);
    analogSignal[SOUND_RU42] = ru42->getSoundSignal(Relay::CHANGE_SOUND);
    //analogSignal[SOUND_RU7] = ru7->getSoundSignal(Relay::CHANGE_SOUND);
    analogSignal[SOUND_RU15] = ru15->getSoundSignal(Relay::CHANGE_SOUND);
    analogSignal[SOUND_MV6] = mv6->getSoundSignal(Relay::CHANGE_SOUND);
    analogSignal[SOUND_VTN] = vtn->getSoundSignal(Relay::CHANGE_SOUND);
    analogSignal[SOUND_RU4] = ru4->getSoundSignal(Relay::CHANGE_SOUND);
    analogSignal[SOUND_RV4] = rv4->getSoundSignal(Relay::CHANGE_SOUND);
    analogSignal[SOUND_RV9] = rv9->getSoundSignal(Relay::CHANGE_SOUND);
    analogSignal[SOUND_KRN] = krn->getSoundSignal(Relay::CHANGE_SOUND);
    analogSignal[SOUND_RU18] = ru18->getSoundSignal(Relay::CHANGE_SOUND);
    analogSignal[SOUND_KTK1] = ktk1->getSoundSignal(Relay::CHANGE_SOUND);
    analogSignal[SOUND_KTK2] = ktk2->getSoundSignal(Relay::CHANGE_SOUND);
    analogSignal[SOUND_RV6] = rv6->getSoundSignal(Relay::CHANGE_SOUND);
    analogSignal[SOUND_KVV] = kvv->getSoundSignal(Relay::CHANGE_SOUND);
    analogSignal[SOUND_KVG] = kvg->getSoundSignal(Relay::CHANGE_SOUND);
    analogSignal[SOUND_KSH1] = ksh1->getSoundSignal(Relay::CHANGE_SOUND);
    analogSignal[SOUND_KSH2] = ksh2->getSoundSignal(Relay::CHANGE_SOUND);
    analogSignal[SOUND_RU1] = ru1->getSoundSignal(Relay::CHANGE_SOUND);

    // Песочница
    analogSignal[SOUND_SAND_DELIVERY] = sand_system->getSoundSignal();
}
