#include    "tep70.h"

#include    <tep70-controls.h>


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
        std::uint16_t dp = (CAB2_BUTTON_WHISTLE_POM - CAB1_BUTTON_WHISTLE_POM) * cab_idx;

        // Освещение
        // Прожектор
        analogSignal[SPOTLIGHT_FWD + d] = spotlight_signal[cab_idx];

        // Буферные огни
        analogSignal[BUFFERLIGHT_FWD_L_WHITE + d] = buf_lights_left[cab_idx][BUF_WHITE];
        analogSignal[BUFFERLIGHT_FWD_L_RED + d] = buf_lights_left[cab_idx][BUF_RED];;
        analogSignal[BUFFERLIGHT_FWD_R_WHITE + d] = buf_lights_right[cab_idx][BUF_WHITE];
        analogSignal[BUFFERLIGHT_FWD_R_RED + d] = buf_lights_right[cab_idx][BUF_RED];

        // Освещение кабины
        analogSignal[CAB1_LIGHT_CABINE + d] = cab_light_signal[cab_idx];
        // Освещение приборов
        analogSignal[CAB1_LIGHT_DEVICES + d] = gauges_light_signal[cab_idx];

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

        analogSignal[CAB1_SIGLIGHT_OIL_TEMP + d] = getLampState(hs_p(max(disel->getWaterTemp() - disel->getWaterTempAlarm(),
                                                                    disel->getOilTemp() - disel->getOilTempAlarm())));

        analogSignal[CAB1_SIGLIGHT_EPB_BRAKE + d] = static_cast<float>(epb_control->stateBrakeLamp());
        analogSignal[CAB1_SIGLIGHT_EPB_HOLD + d] = static_cast<float>(epb_control->stateHoldLamp());
        analogSignal[CAB1_SIGLIGHT_EPB_CONTROL + d] = static_cast<float>(epb_control->stateReleaseLamp());

        analogSignal[CAB1_SIGLIGHT_RESET_POWER + d] = 0.0f;
        analogSignal[CAB1_SIGLIGHT_OIL_PRESS + d] = getLampState(hs_p(0.1 - disel->getOilPressure()));
        analogSignal[CAB1_SIGLIGHT_ZHAL_ET1 + d] = 0.0f;
        analogSignal[CAB1_SIGLIGHT_ZHAL_ET2 + d] = 0.0f;
        analogSignal[CAB1_SIGLIGHT_NO_BATTERY_CHARGE + d] = getLampState(hs_p(100.0 - starter_generator->getVoltage()));

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

        analogSignal[CAB1_STRELKA_WATER_TEMP + d] = static_cast<float>(disel->getWaterTemp() / 120.0);
        analogSignal[CAB1_STRELKA_OIL_TEMP + d] = static_cast<float>(disel->getOilTemp() / 120.0);
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
        analogSignal[CAB1_KM_REVERSOR_POS + d] = static_cast<float>(km[cab_idx]->getReversHandlePos());
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
        analogSignal[CAB1_TUMBLER_FIELD_WEAK1 + d] = tumbler_field_weak1[cab_idx].getPosition();
        analogSignal[CAB1_TUMBLER_FIELD_WEAK2 + d] = tumbler_field_weak2[cab_idx].getPosition();
        analogSignal[CAB1_TUMBLER_WATER_ZALUZI + d] = tumbler_water_zaluzi[cab_idx].getPosition();
        analogSignal[CAB1_TUMBLER_OIL_ZALUZI + d] = tumbler_oil_zaluzi[cab_idx].getPosition();

        // Тумблеры на пульте помощника
        analogSignal[CAB1_TUMBLER_VOLTMETER + d] = static_cast<float>(tumbler_voltage[cab_idx].getState());
        analogSignal[CAB1_TUMBLER_DISEL_STOP + d] = static_cast<float>(tumbler_disel_stop[cab_idx].getState());
        analogSignal[CAB1_TUMBLER_BUFFERLIGHT_L + d] = tumbler_bufferliht_L[cab_idx].getHandlePosition();
        analogSignal[CAB1_TUMBLER_BUFFERLIGHT_R + d] = tumbler_bufferliht_R[cab_idx].getHandlePosition();
        analogSignal[CAB1_TUMBLER_NUMBER_LIGHT + d] = static_cast<float>(tumbler_number_light[cab_idx].getState());
        analogSignal[CAB1_TUMBLER_PANEL_POM_LIGHT + d] = static_cast<float>(tumbler_panel_pom_light[cab_idx].getState());
        analogSignal[CAB1_TUMBLER_RESERVE_LIGHT + d] = static_cast<float>(tumbler_reserve_light[cab_idx].getState());
        analogSignal[CAB1_TUMBLER_CAB_HEATER + d] = static_cast<float>(tumbler_cab_heater[cab_idx].getState());
        analogSignal[CAB1_TUMBLER_CAB_FAN + d] = static_cast<float>(tumbler_cab_fan[cab_idx].getState());

        // Тумблер ПСС — декоративный, анимация от состояния контрола
        analogSignal[CAB1_TUMBLER_PSS + d] = static_cast<float>(tumbler_pss[cab_idx].getState());

        // Кнопки на панели машиниста
        analogSignal[CAB1_BUTTON_DISEL_START + d] = static_cast<float>(button_disel_start[cab_idx].getState());
        analogSignal[CAB1_BUTTON_BRAKE_RELEASE + d] = static_cast<float>(button_brake_release[cab_idx].getState());
        analogSignal[CAB1_BUTTON_CALL_FROM_MASHINERY + d] = 0.0f;
        analogSignal[CAB1_BUTTON_SAND + d] = static_cast<float>(button_sand[cab_idx].getState());
        analogSignal[CAB1_BUTTON_TIFON + d] = static_cast<float>(horn->isTifon());
        analogSignal[CAB1_BUTTON_WHISTLE + d] = static_cast<float>(horn->isSvistok());

        analogSignal[CAB1_BUTTON_WHISTLE_POM + dp] = static_cast<float>(button_svistok_pom[cab_idx].getState());
        analogSignal[CAB1_BUTTON_TIFON_POM + dp] = static_cast<float>(button_tifon_pom[cab_idx].getState());
        analogSignal[CAB1_BUTTON_RB + d] = static_cast<float>(rb[cab_idx][RB].getState());

        analogSignal[CAB1_BUTTON_RBS + d] = static_cast<float>(rb[cab_idx][RBS].getState());
        analogSignal[CAB1_BUTTON_RBP + d] = static_cast<float>(rb[cab_idx][RBP].getState());
        analogSignal[CAB1_EMERGENCY_STOP + d] = 0.0f;

        analogSignal[CAB1_TUMBLER_HEATER_POS + cab_idx] = static_cast<float>(tumbler_heater[cab_idx].getState());
        analogSignal[CAB1_AZV_VENT_POS + cab_idx] = static_cast<float>(azv_ventilation[cab_idx].getState());
        analogSignal[CAB1_GAUGES_LIGHT_POS + cab_idx] = control_inputs[cab_idx][CTRL_GAUGES_LIGHT].value;
        analogSignal[CAB1_TIMETABLE_LIGHT_POS + cab_idx] = control_inputs[cab_idx][CTRL_TIMETABLE_LIGHT].value;
        analogSignal[CAB1_TUMBLER_CABINE_LIGHT_POS + cab_idx] = static_cast<float>(tumbler_cabine_light[cab_idx].getState());
    }

    // Анимации двери и шторы ВВК. Нет смысла пока делать схемный элемент,
    // напрямую транслируем положение контрола в сигнал анимации
    analogSignal[VVK_BLINDS_DOOR_ACTION] = control_inputs[CABS_NUM][CTRL_VVK_BLINDS_DOOR].value;
    analogSignal[VVK_DOOR_ACTION] = control_inputs[CABS_NUM][CTRL_VVK_DOOR].value;

    // Положение АЗВ компрессора (общий, для ВВК)
    analogSignal[AZV_MOTOR_COMPRESSOR_POS] = static_cast<float>(azv_motor_compressor.getState());

    // Положения АЗВ ВВК (общие)
    analogSignal[AZV_BUFFER_LIGHTS_POS] = static_cast<float>(azv_buffer_lights.getState());
    analogSignal[AZV_CONTROL_CIRCUIT_POS] = static_cast<float>(azv_control_circuit.getState());
    analogSignal[AZV_DISEL_LIGHT_POS] = static_cast<float>(azv_disel_light.getState());
    analogSignal[AZV_EPK_POWER_MINUS_POS] = static_cast<float>(azv_epk_power_minus.getState());
    analogSignal[AZV_EPK_POWER_PLUS_POS] = static_cast<float>(azv_epk_power_plus.getState());
    analogSignal[AZV_FIRE_ALARM_POS] = static_cast<float>(azv_fire_alarm.getState());
    analogSignal[AZV_FUEL_PUMP_POS] = static_cast<float>(azv_fuel_pump_vvk.getState());
    analogSignal[AZV_HEATER_POS] = static_cast<float>(azv_heater.getState());
    analogSignal[AZV_SOCKETS_POWER_POS] = static_cast<float>(azv_sockets_power.getState());
    analogSignal[AZV_SOCKETS_POWER_2_POS] = static_cast<float>(azv_sockets_power_2.getState());
    analogSignal[AZV_TROLLEYS_LIGHT_POS] = static_cast<float>(azv_trolleys_light.getState());
    analogSignal[AZV_VVK_LIGHT_POS] = static_cast<float>(azv_vvk_light.getState());
    analogSignal[AZV_WATCH_DISEL_LIGHT_POS] = static_cast<float>(azv_watch_disel_light.getState());

    // Положения органов панели ВВК
    analogSignal[VVK_EDT_TUMBLER_POS] = static_cast<float>(edt_tumbler.getState());
    analogSignal[VVK_SHUNTING_TUMBLER_POS] = static_cast<float>(shunting_tumbler.getState());
    analogSignal[VVK_MANUAL_FUEL_PUMP_POS] = static_cast<float>(manual_fuel_pumping.getState());
    analogSignal[VVK_RESERVED_FUEL_PUMP_POS] = static_cast<float>(reserved_fuel_pump.getState());
    analogSignal[VVK_AUTOSTOP_TUMBLER_POS] = static_cast<float>(autostop_tumbler.getState());
    analogSignal[VVK_ISOLATION_CHECK_POS] = static_cast<float>(isolation_check.getState());
    analogSignal[VVK_FOUND_GROUND_SWITCH_POS] = found_ground_switch.getPosition();
    analogSignal[VVK_GROUND_RELAY_BUTTON_POS] = static_cast<float>(ground_relay_button.getState());
    analogSignal[VVK_KEEP_FUEL_PUMP_BUTTON_POS] = static_cast<float>(keep_fuel_pump_button.getState());
    analogSignal[VVK_V_MINUS_BUTTON_POS] = static_cast<float>(v_minus_button.getState());
    analogSignal[VVK_V_PLUS_BUTTON_POS] = static_cast<float>(v_plus_button.getState());

    // Положения тумблеров ОМ
    for (size_t i = 0; i < NUM_MOTORS; ++i)
        analogSignal[TUMBLER_OM1_POS + i] = static_cast<float>(tumbler_om[i].getState());

    // Положения переключателей ВВК
    for (size_t i = 0; i < VVK_SW_NUM; ++i)
        analogSignal[VVK_SW_PA1_POS + i] = static_cast<float>(vvk_sw[i].getState());

    // Положение тумблера числа тепловозов
    analogSignal[TUMBLER_LOCO_COUNT_POS] = static_cast<float>(tumbler_loco_count.getState());

    // Положение рубильника АКБ
    analogSignal[ACCUM_SWITCH_POS] = static_cast<float>(accum_switch.getState());

    analogSignal[CABINE1_DOOR_ACTION] = 2.0f * control_inputs[CABS_NUM][CTRL_CABINE1_DOOR].value;
    analogSignal[CABINE2_DOOR_ACTION] = 2.0f * control_inputs[CABS_NUM][CTRL_CABINE2_DOOR].value;

    // Жалюзи
    analogSignal[TOP_BLINDS_ACTION] = (water_blinds_open || oil_blinds_open) ? 1.0f : 0.0f;
    analogSignal[EDT_BLINDS_ACTION] = 0.0f;
    analogSignal[WATER_BLINDS_ACTION] = water_blinds_open ? 1.0f : 0.0f;
    analogSignal[OIL_BLINDS_ACTION] = oil_blinds_open ? 1.0f : 0.0f;

    // Вращение вентиляторов охлаждения
    analogSignal[COOLER_FAN_HOT_ROTATION] = static_cast<float>(fan_hot_aero->getAngle() / (2.0 * Physics::PI));
    analogSignal[COOLER_FAN_COLD_ROTATION] = static_cast<float>(fan_cold_aero->getAngle() / (2.0 * Physics::PI));
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
float TEP70::getLampState(double signal)
{
    bool state = (azv_common_control[CAB1].getState() || azv_common_control[CAB2].getState()) &&
                 static_cast<bool>(signal);

    return static_cast<float>(state * power_level);
}
