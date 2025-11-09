#include    "tep70.h"


void TEP70::signalsOutput(const simulator_time_t& t, const double& dt)
{
    (void) t;
    (void) dt;

    analogSignal[SERIAL_NUMBER] = 132.0f;

    analogSignal[WHEELSET_1] = static_cast<float>(wheel_rotation_angle[0] / 2.0 / Physics::PI);
    analogSignal[WHEELSET_2] = static_cast<float>(wheel_rotation_angle[1] / 2.0 / Physics::PI);
    analogSignal[WHEELSET_3] = static_cast<float>(wheel_rotation_angle[2] / 2.0 / Physics::PI);
    analogSignal[WHEELSET_4] = static_cast<float>(wheel_rotation_angle[3] / 2.0 / Physics::PI);
    analogSignal[WHEELSET_5] = static_cast<float>(wheel_rotation_angle[4] / 2.0 / Physics::PI);
    analogSignal[WHEELSET_6] = static_cast<float>(wheel_rotation_angle[5] / 2.0 / Physics::PI);

    // Подсветка номерных знаков
    analogSignal[NUMBER_LIGHT] = static_cast<float>(tumbler_number_light[CAB1].getState() ||
                                                    tumbler_number_light[CAB2].getState());

    // Поворот часовой и минутной стрелки на скоростемерах
    analogSignal[TIME_3SL2M_HOUR] = static_cast<float>(t.time.hour()) + static_cast<float>(t.time.minute()) / 60.0f;
    analogSignal[TIME_3SL2M_MINUTE] = static_cast<float>(t.time.minute()) + static_cast<float>(t.time.sec()) / 60.0f;

    // Кабины
    for (auto cab_idx : {CAB1, CAB2})
    {
        std::uint16_t d = (SPOTLIGHT_BWD - SPOTLIGHT_FWD) * cab_idx;

        // Освещение
        // Прожектор
        if (azv_spotlight_low[cab_idx].getState())
        {
            std::uint8_t intensity = 1 + azv_spotlight_high[cab_idx].getState();
            analogSignal[SPOTLIGHT_FWD + d] = static_cast<float>(intensity) / 2.0f;
        }
        else
        {
            analogSignal[SPOTLIGHT_FWD + d] = 0.0f;
        }

        // Буферные огни
        analogSignal[BUFFERLIGHT_FWD_L_WHITE + d] = static_cast<float>(tumbler_bufferliht_L[cab_idx].isSwitched(2));
        analogSignal[BUFFERLIGHT_FWD_L_RED + d] = static_cast<float>(tumbler_bufferliht_L[cab_idx].isSwitched(0));
        analogSignal[BUFFERLIGHT_FWD_R_WHITE + d] = static_cast<float>(tumbler_bufferliht_R[cab_idx].isSwitched(2));
        analogSignal[BUFFERLIGHT_FWD_R_RED + d] = static_cast<float>(tumbler_bufferliht_R[cab_idx].isSwitched(0));
        analogSignal[CAB1_LIGHT_CABINE + d] = static_cast<float>(azv_cabine_light[cab_idx].getState());
        analogSignal[CAB1_LIGHT_DEVICES + d] = static_cast<float>(azv_panel_light[cab_idx].getState());

        analogSignal[CAB1_LS_WHITE + d] = safety_device[cab_idx]->getWhiteLamp();
        analogSignal[CAB1_LS_RED + d] = safety_device[cab_idx]->getRedLamp();
        analogSignal[CAB1_LS_REDYELLOW + d] = safety_device[cab_idx]->getRedYellowLamp();
        analogSignal[CAB1_LS_YELLOW + d] = safety_device[cab_idx]->getYellowLamp();
        analogSignal[CAB1_LS_GREEN + d] = safety_device[cab_idx]->getGreenLamp();

        // Сигнальные лампы УКБМ
        analogSignal[CAB1_SIGLIGHT_PROP + d] = 0.0f;
        analogSignal[CAB1_SIGLIGHT_PSS + d] = safety_device[cab_idx]->getStatePSS();

        // Сигнальные лампы на пульте машиниста
        analogSignal[CAB1_SIGLIGHT_EDT1 + d] = 0.0f;
        analogSignal[CAB1_SIGLIGHT_EDT2 + d] = 0.0f;

        analogSignal[CAB1_SIGLIGHT_EPB_BRAKE + d] = static_cast<float>(epb_control->stateBrakeLamp());
        analogSignal[CAB1_SIGLIGHT_EPB_HOLD + d] = static_cast<float>(epb_control->stateHoldLamp());
        analogSignal[CAB1_SIGLIGHT_EPB_CONTROL + d] = static_cast<float>(epb_control->stateReleaseLamp());

        analogSignal[CAB1_SIGLIGHT_RESET_POWER + d] = 0.0f;
        analogSignal[CAB1_SIGLIGHT_OIL_TEMP + d] = 0.0f;
        analogSignal[CAB1_SIGLIGHT_OIL_PRESS + d] = getLampState(hs_p(0.1 - disel->getOilPressure()));
        analogSignal[CAB1_SIGLIGHT_ZHAL_ET1 + d] = 0.0f;
        analogSignal[CAB1_SIGLIGHT_ZHAL_ET2 + d] = 0.0f;
        analogSignal[CAB1_SIGLIGHT_NO_BATTERY_CHARGE] = getLampState(hs_p(100.0 - starter_generator->getVoltage()));

        // Сигнальные лампы на пульте помощника
        analogSignal[CAB1_SIGLIGHT_RES_FUELPUMP + d] = 0.0f;
        analogSignal[CAB1_SIGLIGHT_FIRE_L + d] = 0.0f;
        analogSignal[CAB1_SIGLIGHT_FIRE_R + d] = 0.0f;
        analogSignal[CAB1_SIGLIGHT_BRAKE_RESISTORS + d] = 0.0f;
        analogSignal[CAB1_SIGLIGHT_KALORIPHER + d] = 0.0f;

        // Циферблаты на пульте машиниста
        analogSignal[CAB1_STRELKA_GEN_CURRENT + d] = static_cast<float>(I_gen / 10000.0);
        analogSignal[CAB1_STRELKA_REOSTATE_CURRENT + d] = 0.0f;
        analogSignal[CAB1_STRELKA_REF_SPEED + d] = 0.0f;

        analogSignal[CAB1_PRESSURE_BC1 + d] = static_cast<float>(brake_mech[TROLLEY_FWD]->getBCpressure() / 1.0);
        analogSignal[CAB1_PRESSURE_BC2 + d] = static_cast<float>(brake_mech[TROLLEY_BWD]->getBCpressure() / 1.0);
        analogSignal[CAB1_PRESSURE_FL + d] = static_cast<float>(main_reservoir->getPressure() / 1.6);
        analogSignal[CAB1_PRESSURE_BP + d] = static_cast<float>(brakepipe->getPressure() / 1.6);
        analogSignal[CAB1_PRESSURE_ER + d] = static_cast<float>(brake_crane[cab_idx]->getERpressure() / 1.0);

        analogSignal[CAB1_STRELKA_WATER_TEMP + d] = 0.0;
        analogSignal[CAB1_STRELKA_OIL_TEMP + d] = 0.0;
        analogSignal[CAB1_STRELKA_OIL_PRESS + d] = static_cast<float>(disel->getOilPressure() * Physics::g / 15.0);
        analogSignal[CAB1_STRELKA_FUEL_PRESS + d] = static_cast<float>(electro_fuel_pump->getFuelPressure() * Physics::g / 15.0);

        // Циферблаты на пульте помощника
        const double U_bat = tumbler_voltage[cab_idx].getState() ? epb_converter->getOutputVoltage() : Ucc;
        analogSignal[CAB1_STRELKA_BAT_VOLTAGE + d] = static_cast<float>(U_bat / 150.0);
        analogSignal[CAB1_STRELKA_BAT_CURRENT + d] = static_cast<float>(battery->getChargeCurrent() / 150.0);
        analogSignal[CAB1_STRELKA_GEN_VOLTAGE + d] = static_cast<float>(trac_gen->getVoltage() / 1000.0);
        analogSignal[CAB1_STRELKA_FAILS_INDICATOR + d] = 0.0;

        // Скоростемер
        analogSignal[CAB1_3SL2M_SPEED + d] = speed_meter[cab_idx]->getArrowPos();
        analogSignal[CAB1_3SL2M_SHAFT + d] = speed_meter[cab_idx]->getShaftPos();

        // Контроллер
        analogSignal[CAB1_IS_REVERS_HANDLE + d] = static_cast<float>(km[cab_idx]->isReversHandle());
        analogSignal[CAB1_KM_REVERSOR_POS + d] = static_cast<float>(km[cab_idx]->getReversState());
        analogSignal[CAB1_KM_SHTURVAL_POS + d] = km[cab_idx]->getMainShaftPos();
        analogSignal[CAB1_BRAKE_FORCE_SWITCH + d] = brake_force_switch[cab_idx].getHandlePosition();

        // Приборы управления тормозами
        analogSignal[CAB1_AUTOSTOP_IS_KEY + d] = static_cast<float>(epk[cab_idx]->isKey());
        analogSignal[CAB1_AUTOSTOP_KEY_POS + d] = static_cast<float>(epk[cab_idx]->isKeyOn());
        analogSignal[CAB1_UBT_IS_KEY_HANDLE + d] = static_cast<float>(brake_lock[cab_idx]->isLockHandle());
        analogSignal[CAB1_UBT_KEY_HANDLE_POS + d] = static_cast<float>(brake_lock[cab_idx]->getLockHandlePosition());
        analogSignal[CAB1_UBT_COMBINE_CRANE_POS + d] = static_cast<float>(brake_lock[cab_idx]->getCombineCraneHandlePosition());
        analogSignal[CAB1_BRAKE_CRANE_HANDLE_POS + d] = static_cast<float>(brake_crane[cab_idx]->getHandlePosition());
        analogSignal[CAB1_LOCO_CRANE_HANDLE_POS + d] = static_cast<float>(loco_crane[cab_idx]->getHandlePosition());

        // Автоматические защитные выключатели на пульте машиниста
        analogSignal[CAB1_AZV_CABINE_LIGHT + d] = static_cast<float>(azv_cabine_light[cab_idx].getState());
        analogSignal[CAB1_AZV_PANEL_LIGHT + d] = static_cast<float>(azv_panel_light[cab_idx].getState());
        analogSignal[CAB1_AZV_COMMON_CONTROL + d] = static_cast<float>(azv_common_control[cab_idx].getState());
        analogSignal[CAB1_AZV_LOCO_CONTROL + d] = static_cast<float>(azv_upr_tepl[cab_idx].getState());
        analogSignal[CAB1_AZV_FUEL_PUMP + d] = static_cast<float>(azv_fuel_pump[cab_idx].getState());
        analogSignal[CAB1_AZV_EDT_ON + d] = static_cast<float>(azv_edt_on[cab_idx].getState());
        analogSignal[CAB1_AZV_EPB_ON + d] = static_cast<float>(azv_ept_on[cab_idx].getState());
        analogSignal[CAB1_AZV_EPB_POWER + d] = static_cast<float>(azv_ept_power[cab_idx].getState());
        analogSignal[CAB1_AZV_SPOTLIGHT_LOW + d] = static_cast<float>(azv_spotlight_low[cab_idx].getState());
        analogSignal[CAB1_AZV_SPOTLIGHT_HIGH + d] = static_cast<float>(azv_spotlight_high[cab_idx].getState());

        // Тумблеры на пульте машиниста
        analogSignal[CAB1_TUMBLER_FIELD_WEAK1 + d] = tumbler_field_weak1[cab_idx].getHandlePosition();
        analogSignal[CAB1_TUMBLER_FIELD_WEAK2 + d] = tumbler_field_weak2[cab_idx].getHandlePosition();
        analogSignal[CAB1_TUMBLER_WATER_ZALUZI + d] = tumbler_water_zaluzi[cab_idx].getHandlePosition();
        analogSignal[CAB1_TUMBLER_OIL_ZALUZI + d] = tumbler_oil_zaluzi[cab_idx].getHandlePosition();

        // Тумблеры на пульте помощника
        analogSignal[CAB1_TUMBLER_VOLTMETER + d] = static_cast<float>(tumbler_voltage[cab_idx].getState());
        analogSignal[CAB1_TUMBLER_DISEL_STOP + d] = static_cast<float>(tumbler_disel_stop[cab_idx].getState());
        analogSignal[CAB1_TUMBLER_BUFFERLIGHT_L + d] = tumbler_bufferliht_L[cab_idx].getHandlePosition();
        analogSignal[CAB1_TUMBLER_BUFFERLIGHT_R + d] = tumbler_bufferliht_R[cab_idx].getHandlePosition();
        analogSignal[CAB1_TUMBLER_NUMBER_LIGHT + d] = static_cast<float>(tumbler_number_light[cab_idx].getState());
        analogSignal[CAB1_TUMBLER_PANEL_POM_LIGHT + d] = 0.0f;
        analogSignal[CAB1_TUMBLER_RESERVE_LIGHT + d] = 0.0f;
        analogSignal[CAB1_TUMBLER_CAB_HEATER + d] = 0.0f;
        analogSignal[CAB1_TUMBLER_CAB_FAN + d] = 0.0f;

        // Кнопки на панели машиниста
        analogSignal[CAB1_BUTTON_DISEL_START + d] = static_cast<float>(button_disel_start[cab_idx].getState());
        analogSignal[CAB1_BUTTON_BRAKE_RELEASE + d] = static_cast<float>(button_brake_release[cab_idx].getState());
        analogSignal[CAB1_BUTTON_CALL_FROM_MASHINERY + d] = 0.0f;
        analogSignal[CAB1_BUTTON_SAND + d] = static_cast<float>(sand_system->isSandDelivery());
        analogSignal[CAB1_BUTTON_TIFON + d] = static_cast<float>(horn->isTifon());
        analogSignal[CAB1_BUTTON_WHISTLE + d] = static_cast<float>(horn->isSvistok());
        analogSignal[CAB1_BUTTON_RB + d] = static_cast<float>(rb[cab_idx][RB].getState());

        analogSignal[CAB1_BUTTON_RBS + d] = 0.0f;
        analogSignal[CAB1_BUTTON_RBP + d] = 0.0f;
        analogSignal[CAB1_EMERGENCY_STOP + d] = 0.0f;
    }
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
float TEP70::getLampState(double signal)
{
    bool state = (azv_common_control[CAB1].getState() || azv_common_control[CAB2].getState()) &&
                 static_cast<bool>(signal);

    return static_cast<float>(state);
}
