#include    <tep70.h>
#include    <tep70-controls.h>

#include    <cmath>

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void TEP70::stepCommonControls(double t, double dt)
{
    const size_t shared_cab = vc_inputs.size() - 1;

    // Расцепные рычаги: фиксация в расцепленном положении либо аналоговое положение
    if (vc_inputs.getBool(shared_cab, CTRL_OPER_ROD_FIX_FWD))
        oper_rod_fwd->setExternalState(-1.0);
    else
        oper_rod_fwd->setExternalState(vc_inputs.getValue(shared_cab, CTRL_OPER_ROD_FWD));

    if (vc_inputs.getBool(shared_cab, CTRL_OPER_ROD_FIX_BWD))
        oper_rod_bwd->setExternalState(-1.0);
    else
        oper_rod_bwd->setExternalState(vc_inputs.getValue(shared_cab, CTRL_OPER_ROD_BWD));

    bool ac_bc_fwd = vc_inputs.getBool(shared_cab, CTRL_ANGLECOCK_BC_FWD);
    ac_bc_fwd ? anglecock_bc_fwd->open() : anglecock_bc_fwd->close();

    bool ac_bc_bwd = vc_inputs.getBool(shared_cab, CTRL_ANGLECOCK_BC_BWD);
    ac_bc_bwd ? anglecock_bc_bwd->open() : anglecock_bc_bwd->close();

    // Концевые краны питательной магистрали
    bool ac_fl_fwd = vc_inputs.getBool(shared_cab, CTRL_ANGLECOCK_FL_FWD);
    ac_fl_fwd ? anglecock_fl_fwd->open() : anglecock_fl_fwd->close();

    bool ac_fl_bwd = vc_inputs.getBool(shared_cab, CTRL_ANGLECOCK_FL_BWD);
    ac_fl_bwd ? anglecock_fl_bwd->open() : anglecock_fl_bwd->close();

    // Концевые краны тормозной магистрали
    bool ac_bp_fwd = vc_inputs.getBool(shared_cab, CTRL_ANGLECOCK_BP_FWD);
    ac_bp_fwd ? anglecock_bp_fwd->open() : anglecock_bp_fwd->close();

    bool ac_bp_bwd = vc_inputs.getBool(shared_cab, CTRL_ANGLECOCK_BP_BWD);
    ac_bp_bwd ? anglecock_bp_bwd->open() : anglecock_bp_bwd->close();

    // Рукава магистралей: 0 - разъединён, 2 - соединён (1 - нейтраль)
    hose_bc_fwd->setExternalState(static_cast<int>(std::round(vc_inputs.getValue(shared_cab, CTRL_HOSE_BC_FWD))));
    hose_bc_bwd->setExternalState(static_cast<int>(std::round(vc_inputs.getValue(shared_cab, CTRL_HOSE_BC_BWD))));
    hose_fl_fwd->setExternalState(static_cast<int>(std::round(vc_inputs.getValue(shared_cab, CTRL_HOSE_FL_FWD))));
    hose_fl_bwd->setExternalState(static_cast<int>(std::round(vc_inputs.getValue(shared_cab, CTRL_HOSE_FL_BWD))));
    hose_bp_fwd->setExternalState(static_cast<int>(std::round(vc_inputs.getValue(shared_cab, CTRL_HOSE_BP_FWD))));
    hose_bp_bwd->setExternalState(static_cast<int>(std::round(vc_inputs.getValue(shared_cab, CTRL_HOSE_BP_BWD))));

    // АЗВ "Компрессор" (общий)
    bool azv_motor_comp_on = vc_inputs.getBool(shared_cab, CTRL_AZV_MOTOR_COMPRESSOR);
    azv_motor_comp_on ? azv_motor_compressor.set() : azv_motor_compressor.reset();

    // АЗВ ВВК (общие)
    vc_inputs.getBool(shared_cab, CTRL_AZV_BUFFER_LIGHTS)
        ? azv_buffer_lights.set() : azv_buffer_lights.reset();

    vc_inputs.getBool(shared_cab, CTRL_AZV_CONTROL_CIRCUIT)
        ? azv_control_circuit.set() : azv_control_circuit.reset();

    vc_inputs.getBool(shared_cab, CTRL_AZV_DISEL_LIGHT)
        ? azv_disel_light.set() : azv_disel_light.reset();

    vc_inputs.getBool(shared_cab, CTRL_AZV_EPK_POWER_MINUS)
        ? azv_epk_power_minus.set() : azv_epk_power_minus.reset();

    vc_inputs.getBool(shared_cab, CTRL_AZV_EPK_POWER_PLUS)
        ? azv_epk_power_plus.set() : azv_epk_power_plus.reset();

    vc_inputs.getBool(shared_cab, CTRL_AZV_FIRE_ALARM)
        ? azv_fire_alarm.set() : azv_fire_alarm.reset();

    vc_inputs.getBool(shared_cab, CTRL_AZV_FUEL_PUMP_VVK)
        ? azv_fuel_pump_vvk.set() : azv_fuel_pump_vvk.reset();

    vc_inputs.getBool(shared_cab, CTRL_AZV_HEATER)
        ? azv_heater.set() : azv_heater.reset();

    vc_inputs.getBool(shared_cab, CTRL_AZV_SOCKETS_POWER)
        ? azv_sockets_power.set() : azv_sockets_power.reset();

    vc_inputs.getBool(shared_cab, CTRL_AZV_SOCKETS_POWER_2)
        ? azv_gauges_power.set() : azv_gauges_power.reset();

    vc_inputs.getBool(shared_cab, CTRL_AZV_TROLLEYS_LIGHT)
        ? azv_trolleys_light.set() : azv_trolleys_light.reset();

    vc_inputs.getBool(shared_cab, CTRL_AZV_VVK_LIGHT)
        ? azv_vvk_light.set() : azv_vvk_light.reset();

    vc_inputs.getBool(shared_cab, CTRL_AZV_WATCH_DISEL_LIGHT)
        ? azv_watch_disel_light.set() : azv_watch_disel_light.reset();

    // Органы панели ВВК
    vc_inputs.getBool(shared_cab, CTRL_VVK_EDT_TUMBLER)
        ? edt_tumbler.set() : edt_tumbler.reset();

    vc_inputs.getBool(shared_cab, CTRL_VVK_SHUNTING_TUMBLER)
        ? shunting_tumbler.set() : shunting_tumbler.reset();

    vc_inputs.getBool(shared_cab, CTRL_VVK_MANUAL_FUEL_PUMP)
        ? manual_fuel_pumping.set() : manual_fuel_pumping.reset();

    vc_inputs.getBool(shared_cab, CTRL_VVK_RESERVED_FUEL_PUMP)
        ? reserved_fuel_pump.set() : reserved_fuel_pump.reset();

    vc_inputs.getBool(shared_cab, CTRL_VVK_AUTOSTOP_TUMBLER)
        ? autostop_tumbler.set() : autostop_tumbler.reset();

    vc_inputs.getBool(shared_cab, CTRL_VVK_ISOLATION_CHECK)
        ? isolation_check.set() : isolation_check.reset();

    found_ground_switch.setNumPositions(21);
    found_ground_switch.setPosition(vc_inputs.getValue(shared_cab, CTRL_VVK_FOUND_GROUND_SWITCH));

    vc_inputs.getBool(shared_cab, CTRL_VVK_GROUND_RELAY_BUTTON)
        ? ground_relay_button.set() : ground_relay_button.reset();

    vc_inputs.getBool(shared_cab, CTRL_VVK_KEEP_FUEL_PUMP_BUTTON)
        ? keep_fuel_pump_button.set() : keep_fuel_pump_button.reset();

    vc_inputs.getBool(shared_cab, CTRL_VVK_V_MINUS_BUTTON)
        ? v_minus_button.set() : v_minus_button.reset();

    vc_inputs.getBool(shared_cab, CTRL_VVK_V_PLUS_BUTTON)
        ? v_plus_button.set() : v_plus_button.reset();

    // Тумблеры ОМ
    for (size_t i = 0; i < NUM_MOTORS; ++i)
    {
        vc_inputs.getBool(shared_cab, CTRL_TUMBLER_OM1 + i)
            ? tumbler_om[i].set() : tumbler_om[i].reset();
    }

    // Переключатели ВВК
    for (size_t i = 0; i < VVK_SW_NUM; ++i)
    {
        vc_inputs.getBool(shared_cab, CTRL_VVK_PA1 + i)
            ? vvk_sw[i].set() : vvk_sw[i].reset();

        vvk_sw[i].step(t, dt);
    }

    // Тумблер числа тепловозов
    vc_inputs.getBool(shared_cab, CTRL_TUMBLER_LOCO_COUNT)
        ? tumbler_loco_count.set() : tumbler_loco_count.reset();

    // Рубильник АКБ
    vc_inputs.getBool(shared_cab, CTRL_ACCUM_SWITCH)
        ? accum_switch.set() : accum_switch.reset();
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void TEP70::stepControls(double t, double dt)
{
    // Взаимные блокировки
    brake_lock[CAB2]->allowLockHandle(!(brake_lock[CAB1]->isLockHandle()));
    brake_lock[CAB1]->allowLockHandle(!(brake_lock[CAB2]->isLockHandle()));

    epk[CAB2]->allowKey(!(epk[CAB1]->isKey()));
    epk[CAB1]->allowKey(!(epk[CAB2]->isKey()));

    km[CAB2]->allowReversHandle(!(km[CAB1]->isReversHandle()));
    km[CAB1]->allowReversHandle(!(km[CAB2]->isReversHandle()));    

    // Контроллер машиниста — шаг для таймеров анимации вращения
    km[CAB1]->step(t, dt);
    km[CAB2]->step(t, dt);

    // Общие сигналы (shared)
    stepCommonControls(t, dt);

    // Не читаем vc_inputs во время процедуры автозапуска/автоостанова
    if (autoStartTimer->isStarted())
        return;

    // Автозапуск/автоостанов по сигналам от IOController
    for (auto cab_idx : {CAB1, CAB2})
    {
        bool autostart = vc_inputs.getBool(cab_idx, CTRL_AUTOSTART_PROGRAM);
        if (autostart)
        {
            if (initAutostartProgram(cab_idx))
                autoStartTimer->start();

            initClientInputSignal(cab_idx, CTRL_AUTOSTART_PROGRAM, 0.0f);
            return;
        }

        bool autostop = vc_inputs.getBool(cab_idx, CTRL_AUTOSTOP_PROGRAM);
        if (autostop)
        {
            if (initAutostopProgram(cab_idx))
                autoStartTimer->start();

            initClientInputSignal(cab_idx, CTRL_AUTOSTOP_PROGRAM, 0.0f);
            return;
        }
    }

    // Применяем сигналы от IOController
    for (auto cab_idx : {CAB1, CAB2})
    {
        // АЗВ (Toggle)
        vc_inputs.getBool(cab_idx, CTRL_AZV_CABINE_LIGHT)
            ? azv_cabine_light[cab_idx].set() : azv_cabine_light[cab_idx].reset();

        vc_inputs.getBool(cab_idx, CTRL_AZV_PANEL_LIGHT)
            ? azv_panel_light[cab_idx].set() : azv_panel_light[cab_idx].reset();

        vc_inputs.getBool(cab_idx, CTRL_AZV_COMMON_CONTROL)
            ? azv_common_control[cab_idx].set() : azv_common_control[cab_idx].reset();

        vc_inputs.getBool(cab_idx, CTRL_AZV_UPR_TEPL)
            ? azv_upr_tepl[cab_idx].set() : azv_upr_tepl[cab_idx].reset();

        vc_inputs.getBool(cab_idx, CTRL_AZV_FUEL_PUMP)
            ? azv_fuel_pump[cab_idx].set() : azv_fuel_pump[cab_idx].reset();

        vc_inputs.getBool(cab_idx, CTRL_AZV_EDT_ON)
            ? azv_edt_on[cab_idx].set() : azv_edt_on[cab_idx].reset();

        vc_inputs.getBool(cab_idx, CTRL_AZV_EPT_ON)
            ? azv_ept_on[cab_idx].set() : azv_ept_on[cab_idx].reset();

        vc_inputs.getBool(cab_idx, CTRL_AZV_EPT_POWER)
            ? azv_ept_power[cab_idx].set() : azv_ept_power[cab_idx].reset();

        vc_inputs.getBool(cab_idx, CTRL_AZV_SPOTLIGHT_LOW)
            ? azv_spotlight_low[cab_idx].set() : azv_spotlight_low[cab_idx].reset();

        vc_inputs.getBool(cab_idx, CTRL_AZV_SPOTLIGHT_HIGH)
            ? azv_spotlight_high[cab_idx].set() : azv_spotlight_high[cab_idx].reset();

        // Тумблеры 3-поз (Switcher)
        tumbler_field_weak1[cab_idx].setPosition(
            vc_inputs.getValue(cab_idx, CTRL_TUMBLER_FIELD_WEAK1));

        tumbler_field_weak2[cab_idx].setPosition(
            vc_inputs.getValue(cab_idx, CTRL_TUMBLER_FIELD_WEAK2));

        tumbler_water_zaluzi[cab_idx].setPosition(
            vc_inputs.getValue(cab_idx, CTRL_TUMBLER_WATER_ZALUZI));

        tumbler_oil_zaluzi[cab_idx].setPosition(
            vc_inputs.getValue(cab_idx, CTRL_TUMBLER_OIL_ZALUZI));

        tumbler_bufferliht_L[cab_idx].setPosition(
            vc_inputs.getValue(cab_idx, CTRL_TUMBLER_BUFFERLIGHT_L));

        tumbler_bufferliht_R[cab_idx].setPosition(
            vc_inputs.getValue(cab_idx, CTRL_TUMBLER_BUFFERLIGHT_R));

        // Тумблеры (Toggle)
        vc_inputs.getBool(cab_idx, CTRL_TUMBLER_VOLTMETER)
            ? tumbler_voltage[cab_idx].set() : tumbler_voltage[cab_idx].reset();

        vc_inputs.getBool(cab_idx, CTRL_TUMBLER_DISEL_STOP)
            ? tumbler_disel_stop[cab_idx].set() : tumbler_disel_stop[cab_idx].reset();

        vc_inputs.getBool(cab_idx, CTRL_TUMBLER_NUMBER_LIGHT)
            ? tumbler_number_light[cab_idx].set() : tumbler_number_light[cab_idx].reset();

        vc_inputs.getBool(cab_idx, CTRL_TUMBLER_PANEL_POM_LIGHT)
            ? tumbler_panel_pom_light[cab_idx].set() : tumbler_panel_pom_light[cab_idx].reset();

        vc_inputs.getBool(cab_idx, CTRL_TUMBLER_RESERVE_LIGHT)
            ? tumbler_reserve_light[cab_idx].set() : tumbler_reserve_light[cab_idx].reset();

        vc_inputs.getBool(cab_idx, CTRL_TUMBLER_CAB_HEATER)
            ? tumbler_cab_heater[cab_idx].set() : tumbler_cab_heater[cab_idx].reset();

        vc_inputs.getBool(cab_idx, CTRL_TUMBLER_CAB_FAN)
            ? tumbler_cab_fan[cab_idx].set() : tumbler_cab_fan[cab_idx].reset();

        // Тумблер ПСС (декоративный)
        vc_inputs.getBool(cab_idx, CTRL_TUMBLER_PSS)
            ? tumbler_pss[cab_idx].set() : tumbler_pss[cab_idx].reset();

        // Тумблеры пульта
        vc_inputs.getBool(cab_idx, CTRL_TUMBLER_GAUGES_POWER)
            ? tumbler_gauges_power[cab_idx].set() : tumbler_gauges_power[cab_idx].reset();

        vc_inputs.getBool(cab_idx, CTRL_TUMBLER_RADIO_KV)
            ? tumbler_radio_kv[cab_idx].set() : tumbler_radio_kv[cab_idx].reset();

        vc_inputs.getBool(cab_idx, CTRL_TUMBLER_RADIO_UKV)
            ? tumbler_radio_ukv[cab_idx].set() : tumbler_radio_ukv[cab_idx].reset();

        vc_inputs.getBool(cab_idx, CTRL_TUMBLER_ALSN_FREQ)
            ? tumbler_alsn_freq[cab_idx].set() : tumbler_alsn_freq[cab_idx].reset();

        // Кнопки пульта
        vc_inputs.getBool(cab_idx, CTRL_BUTTON_MANEUVER)
            ? button_maneuver[cab_idx].set() : button_maneuver[cab_idx].reset();

        vc_inputs.getBool(cab_idx, CTRL_BUTTON_SVISTOK_MANEUVER)
            ? button_svistok_maneuver[cab_idx].set() : button_svistok_maneuver[cab_idx].reset();

        // Кнопки новые
        vc_inputs.getBool(cab_idx, CTRL_BUTTON_CALL_DRIVER_ASSISTANT)
            ? button_call_driver_assistant[cab_idx].set() : button_call_driver_assistant[cab_idx].reset();

        vc_inputs.getBool(cab_idx, CTRL_BUTTON_CHECK_EDT)
            ? button_check_edt[cab_idx].set() : button_check_edt[cab_idx].reset();

        vc_inputs.getBool(cab_idx, CTRL_BUTTON_CHECK_FIRE_ALARM)
            ? button_check_fire_alarm[cab_idx].set() : button_check_fire_alarm[cab_idx].reset();

        vc_inputs.getBool(cab_idx, CTRL_BUTTON_FIND_TROUBLES)
            ? button_find_troubles[cab_idx].set() : button_find_troubles[cab_idx].reset();

        vc_inputs.getBool(cab_idx, CTRL_RESET_SET_PROTECTIONS)
            ? reset_set_protections[cab_idx].set() : reset_set_protections[cab_idx].reset();

        vc_inputs.getBool(cab_idx, CTRL_BUTTON_EMPTY1)
            ? button_empty1[cab_idx].set() : button_empty1[cab_idx].reset();

        vc_inputs.getBool(cab_idx, CTRL_BUTTON_EMPTY2)
            ? button_empty2[cab_idx].set() : button_empty2[cab_idx].reset();

        // Кнопки
        vc_inputs.getBool(cab_idx, CTRL_BUTTON_DISEL_START)
            ? button_disel_start[cab_idx].set() : button_disel_start[cab_idx].reset();

        vc_inputs.getBool(cab_idx, CTRL_BUTTON_BRAKE_RELEASE)
            ? button_brake_release[cab_idx].set() : button_brake_release[cab_idx].reset();

        vc_inputs.getBool(cab_idx, CTRL_BUTTON_SVISTOK)
            ? button_svistok[cab_idx].set() : button_svistok[cab_idx].reset();

        vc_inputs.getBool(cab_idx, CTRL_BUTTON_SVISTOK_POM)
            ? button_svistok_pom[cab_idx].set() : button_svistok_pom[cab_idx].reset();

        vc_inputs.getBool(cab_idx, CTRL_BUTTON_TIFON)
            ? button_tifon[cab_idx].set() : button_tifon[cab_idx].reset();

        vc_inputs.getBool(cab_idx, CTRL_BUTTON_TIFON_POM)
            ? button_tifon_pom[cab_idx].set() : button_tifon_pom[cab_idx].reset();

        vc_inputs.getBool(cab_idx, CTRL_SAND_BUTTON)
            ? button_sand[cab_idx].set() : button_sand[cab_idx].reset();

        // РБ
        vc_inputs.getBool(cab_idx, CTRL_RBS)
            ? rb[cab_idx][RBS].set() : rb[cab_idx][RBS].reset();

        vc_inputs.getBool(cab_idx, CTRL_RB)
            ? rb[cab_idx][RB].set() : rb[cab_idx][RB].reset();

        vc_inputs.getBool(cab_idx, CTRL_RBP)
            ? rb[cab_idx][RBP].set() : rb[cab_idx][RBP].reset();

        // Переключатели
        brake_force_switch[cab_idx].setPosition(
            vc_inputs.getValue(cab_idx, CTRL_BRAKE_FORCE_SWITCH));

        // Автоведение, маневровый режим
        vc_inputs.getBool(cab_idx, CTRL_AUTOPILOT)
            ? autopilot_switcher[cab_idx].set() : autopilot_switcher[cab_idx].reset();

        vc_inputs.getBool(cab_idx, CTRL_SHUNTING_MODE)
            ? shunting_mode_switcher[cab_idx].set() : shunting_mode_switcher[cab_idx].reset();

        // ЭПК
        vc_inputs.getBool(cab_idx, CTRL_EPK_INSERTION)
            ? epk[cab_idx]->insertKey(true) : epk[cab_idx]->insertKey(false);

        vc_inputs.getBool(cab_idx, CTRL_KEY_EPK)
            ? epk[cab_idx]->setKeyOn(true) : epk[cab_idx]->setKeyOn(false);

        // Реверсор, штурвал КМ и тормозные краны.
        // При активном автоведении клиентское управление не применяем,
        // а отдаём клиенту фактическое состояние органов.
        if (!autopilot_switcher[cab_idx].getState())
        {
            // Реверсор и штурвал КМ
            bool revers_insert = vc_inputs.getBool(cab_idx, CTRL_REVERS_INSERTION);
            km[cab_idx]->insertReversHandle(revers_insert);

            int revers_pos = vc_inputs.getValue(cab_idx, CTRL_REVERS_POSITION);
            km[cab_idx]->setReversHandlePos(revers_pos);

            int main_pos = static_cast<int>(vc_inputs.getValue(cab_idx, CTRL_KM_MAIN_POSITION));
            km[cab_idx]->setPosition(main_pos);

            km[cab_idx]->setSelsinPos(vc_inputs.getParam(cab_idx, CTRL_KM_MAIN_POSITION, 0));

            // Тормозные краны
            int krm_pos = vc_inputs.getValue(cab_idx, CTRL_KRM_395);
            brake_crane[cab_idx]->setHandlePosition(krm_pos);

            double kvt_pos = vc_inputs.getValue(cab_idx, CTRL_KVT_254);
            loco_crane[cab_idx]->setHandlePosition(kvt_pos);

            initClientInputSignal(cab_idx, CTRL_REVERS_INSERTION,
                                  km[cab_idx]->isReversHandle() ? 1.0f : 0.0f);
            initClientInputSignal(cab_idx, CTRL_REVERS_POSITION,
                                  km[cab_idx]->getReversHandlePos());
            initClientInputSignal(cab_idx, CTRL_KM_MAIN_POSITION,
                                  km[cab_idx]->getMainShaftPos());
        }
        else
        {
            initClientInputSignal(cab_idx, CTRL_REVERS_INSERTION,
                                  km[cab_idx]->isReversHandle() ? 1.0f : 0.0f);
            initClientInputSignal(cab_idx, CTRL_REVERS_POSITION,
                                  km[cab_idx]->getReversHandlePos());
            initClientInputSignal(cab_idx, CTRL_KM_MAIN_POSITION,
                                  km[cab_idx]->getPositionNumber());
            initClientInputSignal(cab_idx, CTRL_KRM_395,
                                  brake_crane[cab_idx]->getHandlePosition());
            initClientInputSignal(cab_idx, CTRL_KVT_254,
                                  loco_crane[cab_idx]->getHandlePosition());
        }

        // Управление блокировкой УБТ-367
        bool is_lock367_insert = vc_inputs.getBool(cab_idx, CTRL_LOCK_367_INSERTION);
        brake_lock[cab_idx]->insertLockHandle(is_lock367_insert);
        brake_lock[cab_idx]->setStateOn(is_lock367_insert);

        brake_lock[cab_idx]->setCombineCranePosition(
            vc_inputs.getValue(cab_idx, CTRL_COMBINE_KRAN));

        // Тумблер отопления
        vc_inputs.getBool(cab_idx, CTRL_TUMBLER_HEATER)
            ? tumbler_heater[cab_idx].set() : tumbler_heater[cab_idx].reset();

        // АЗВ вентиляции
        vc_inputs.getBool(cab_idx, CTRL_AZV_VENT)
            ? azv_ventilation[cab_idx].set() : azv_ventilation[cab_idx].reset();

        // Тумблер освещения кабины (собственный)
        vc_inputs.getBool(cab_idx, CTRL_TUMBLER_CABINE_LIGHT)
            ? tumbler_cabine_light[cab_idx].set() : tumbler_cabine_light[cab_idx].reset();
    }    
}
