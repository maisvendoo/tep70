#include    <tep70.h>

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void TEP70::soundsOutput(const simulator_time_t& t, const double& dt)
{
    (void) t;
    (void) dt;

    // Озвучка кабин
    for (auto cab_idx : {CAB1, CAB2})
    {
        std::uint16_t d = (SOUND_BWD_SVISTOK - SOUND_FWD_SVISTOK) * cab_idx;

        // Свистулька и тифулька :-)
        analogSignal[SOUND_FWD_SVISTOK + d] = horn->getSoundSignal(TrainHorn::SVISTOK_SOUND);
        analogSignal[SOUND_FWD_TIFON + d] = horn->getSoundSignal(TrainHorn::TIFON_SOUND);

        // Реверсор и контроллер
        analogSignal[CAB1_SOUND_INSERT_REVERS_HANDLE + d] = sound_state_t::createSoundSignal(false);
        analogSignal[CAB1_SOUND_REMOVE_REVERS_HANDLE + d] = sound_state_t::createSoundSignal(false);
        analogSignal[CAB1_SOUND_REVERS_CHANGE_POS + d] = km[cab_idx]->getSoundSignal(ControllerKM2202::REVERS_SHAFT);
        analogSignal[CAB1_SOUND_CONTROLLER_CHANGE_POS + d] = km[cab_idx]->getSoundSignal(ControllerKM2202::MAIN_SHAFT);

        // Устройство блокировки тормозов
        analogSignal[CAB1_SOUND_INSERT_BRAKE_LOCK_HANDLE + d] = brake_lock[cab_idx]->getSoundSignal(PneumoBrakeLock::LOCK_HANDLE_INSERTED);
        analogSignal[CAB1_SOUND_REMOVE_BRAKE_LOCK_HANDLE + d] = brake_lock[cab_idx]->getSoundSignal(PneumoBrakeLock::LOCK_HANDLE_REMOVED);
        analogSignal[CAB1_SOUND_BRAKE_LOCK_CHANGE_LOCK_POS + d] = brake_lock[cab_idx]->getSoundSignal(PneumoBrakeLock::LOCK_STATE_CHANGED);
        analogSignal[CAB1_SOUND_BRAKE_LOCK_CHANGE_COMB_POS + d] = brake_lock[cab_idx]->getSoundSignal(PneumoCombineCrane::CHANGE_COMB_POS_SOUND);
        analogSignal[CAB1_SOUND_BRAKE_LOCK_BP_DRAIN_FLOW + d] = brake_lock[cab_idx]->getSoundSignal(PneumoCombineCrane::BP_DRAIN_FLOW_SOUND);

        // Поездной кран
        analogSignal[CAB1_SOUND_BRAKE_CRANE_CHANGE_POS + d] = brake_crane[cab_idx]->getSoundSignal(BrakeCrane::CHANGE_POS_SOUND);
        analogSignal[CAB1_SOUND_BRAKE_CRANE_ER_STAB_FLOW + d] = brake_crane[cab_idx]->getSoundSignal(BrakeCrane::ER_STAB_SOUND);
        analogSignal[CAB1_SOUND_BRAKE_CRANE_ER_FILL_FLOW + d] = brake_crane[cab_idx]->getSoundSignal(BrakeCrane::ER_FILL_FLOW_SOUND);
        analogSignal[CAB1_SOUND_BRAKE_CRANE_ER_DRAIN_FLOW + d] = brake_crane[cab_idx]->getSoundSignal(BrakeCrane::ER_DRAIN_FLOW_SOUND);
        analogSignal[CAB1_SOUND_BRAKE_CRANE_BP_FILL_FLOW + d] = brake_crane[cab_idx]->getSoundSignal(BrakeCrane::BP_FILL_FLOW_SOUND);
        analogSignal[CAB1_SOUND_BRAKE_CRANE_BP_DRAIN_FLOW + d] = brake_crane[cab_idx]->getSoundSignal(BrakeCrane::BP_DRAIN_FLOW_SOUND);

        // Локомотивный кран
        analogSignal[CAB1_SOUND_LOCO_CRANE_CHANGE_POS + d] = loco_crane[cab_idx]->getSoundSignal(LocoCrane::CHANGE_POS_SOUND);
        analogSignal[CAB1_SOUND_LOCO_CRANE_BC_FILL_FLOW + d] = loco_crane[cab_idx]->getSoundSignal(LocoCrane::BC_FILL_FLOW_SOUND);
        analogSignal[CAB1_SOUND_LOCO_CRANE_BC_DRAIN_FLOW + d] = loco_crane[cab_idx]->getSoundSignal(LocoCrane::BC_DRAIN_FLOW_SOUND);

        // АЗВ на пульте машиниста
        analogSignal[CAB1_SOUND_AZV_CABINE_LIGHT + d] = sound_state_t::createSoundSignal(false);
        analogSignal[CAB1_SOUND_AZV_PANEL_LIGHT + d] = sound_state_t::createSoundSignal(false);
        analogSignal[CAB1_SOUND_AZV_COMMON_CONTROL + d] = azv_common_control[cab_idx].getSoundSignal(Trigger::CHANGE_SOUND);
        analogSignal[CAB1_SOUND_AZV_LOCO_CONTROL + d] = azv_upr_tepl[cab_idx].getSoundSignal(Trigger::CHANGE_SOUND);
        analogSignal[CAB1_SOUND_AZV_FUEL_PUMP + d] = azv_fuel_pump[cab_idx].getSoundSignal(Trigger::CHANGE_SOUND);
        analogSignal[CAB1_SOUND_AZV_EDT_ON + d] = azv_edt_on[cab_idx].getSoundSignal(Trigger::CHANGE_SOUND);
        analogSignal[CAB1_SOUND_AZV_EPB_ON + d] = azv_ept_on[cab_idx].getSoundSignal(Trigger::CHANGE_SOUND);
        analogSignal[CAB1_SOUND_AZV_EPB_POWER + d] = azv_edt_power[cab_idx].getSoundSignal(Trigger::CHANGE_SOUND);
        analogSignal[CAB1_SOUND_AZV_SPOTLIGHT_LOW + d] = sound_state_t::createSoundSignal(false);
        analogSignal[CAB1_SOUND_AZV_SPOTLIGHT_HIGH + d] = sound_state_t::createSoundSignal(false);

        // Тумблеры на пульте машиниста
        analogSignal[CAB1_SOUND_TUMBLER_FIELD_WEAK1 + d] = tumbler_field_weak1[cab_idx].getSoundSignal();
        analogSignal[CAB1_SOUND_TUMBLER_FIELD_WEAK2 + d] = tumbler_field_weak2[cab_idx].getSoundSignal();
        analogSignal[CAB1_SOUND_TUMBLER_WATER_ZALUZI + d] = tumbler_water_zaluzi[cab_idx].getSoundSignal();
        analogSignal[CAB1_SOUND_TUMBLER_OIL_ZALUZI + d] = tumbler_oil_zaluzi[cab_idx].getSoundSignal();

        // Тумблеры на пульте помощника
        analogSignal[CAB1_SOUND_TUMBLER_VOLTMETER + d] = tumbler_voltage[cab_idx].getSoundSignal(Trigger::CHANGE_SOUND);
        analogSignal[CAB1_SOUND_TUMBLER_DISEL_STOP + d] = tumbler_disel_stop[cab_idx].getSoundSignal(Trigger::CHANGE_SOUND);
        analogSignal[CAB1_SOUND_TUMBLER_BUFFERLIGHT_L + d] = sound_state_t::createSoundSignal(false);
        analogSignal[CAB1_SOUND_TUMBLER_BUFFERLIGHT_R + d] = sound_state_t::createSoundSignal(false);
        analogSignal[CAB1_SOUND_TUMBLER_NUMBER_LIGHT + d] = sound_state_t::createSoundSignal(false);
        analogSignal[CAB1_SOUND_TUMBLER_PANEL_POM_LIGHT + d] = sound_state_t::createSoundSignal(false);
        analogSignal[CAB1_SOUND_TUMBLER_RESERVE_LIGHT + d] = sound_state_t::createSoundSignal(false);
        analogSignal[CAB1_SOUND_TUMBLER_CAB_HEATER + d] = sound_state_t::createSoundSignal(false);
        analogSignal[CAB1_SOUND_TUMBLER_CAB_FAN + d] = sound_state_t::createSoundSignal(false);

        // ЭПК
        analogSignal[CAB1_SOUND_EPK_INSERT_KEY + d] = epk[cab_idx]->getSoundSignal(AutoTrainStop::KEY_INSERTED);
        analogSignal[CAB1_SOUND_EPK_KEY_ON + d] = epk[cab_idx]->getSoundSignal(AutoTrainStop::KEY_STATE_ON);
        analogSignal[CAB1_SOUND_EPK_REMOVE_KEY + d] = epk[cab_idx]->getSoundSignal(AutoTrainStop::KEY_REMOVED);
        analogSignal[CAB1_SOUND_EPK_KEY_OFF + d] = epk[cab_idx]->getSoundSignal(AutoTrainStop::KEY_STATE_OFF);
        analogSignal[CAB1_SOUND_AUTOSTOP_WHISTLE + d] = epk[cab_idx]->getSoundSignal(AutoTrainStop::AUTOSTOP_WHISTLE);

        // Переключатель тормозного усилия
        analogSignal[CAB1_SOUND_BRAKE_FORCE_SWITCH + d] = brake_force_switch[cab_idx].getSoundSignal();

        // Скоростемер
        analogSignal[CAB1_SOUND_SKOROSTEMER + d] = speed_meter[cab_idx]->getSoundSignal();
    }

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

    // Дизель
    for (size_t i = 0; i <= 15; ++i)
    {
        analogSignal[SOUND_DISEL_X0 + i] = disel->getSoundSignal(i);
    }

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

    analogSignal[SOUND_AZV_MOTOR_COMPRESSOR] = azv_motor_compressor[CAB1].getSoundSignal(Trigger::CHANGE_SOUND);

    // Мотор-компрессор
    analogSignal[SOUND_MOTOR_COMPRESSOR] = motor_compressor->getSoundSignal();

    // Топливный насос
    analogSignal[SOUND_FUEL_PUMP] = electro_fuel_pump->getSoundSignal();

    // Маслопрокачивающий насос
    analogSignal[SOUND_OIL_PUMP] = electro_oil_pump->getSoundSignal();

    // Стартер-генератор
    analogSignal[SOUND_STARTER_GENERATOR] = starter_generator->getSoundSignal();

    // Песочница
    analogSignal[SOUND_SAND_DELIVERY] = sand_system->getSoundSignal();
}
