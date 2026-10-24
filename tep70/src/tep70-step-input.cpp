#include    <tep70.h>
#include    <tep70-controls.h>

#include    <cmath>

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

    // Переключатель кабины — от сигналов IOController
    if (control_inputs[CAB1][CTRL_CABINE_1].toBool() || control_inputs[CAB2][CTRL_CABINE_1].toBool())
        cabine_switcher->setCabineIndex(CAB1);
    else if (control_inputs[CAB1][CTRL_CABINE_2].toBool() || control_inputs[CAB2][CTRL_CABINE_2].toBool())
        cabine_switcher->setCabineIndex(CAB2);
    cabine_switcher->step(t, dt);

    // Песочница — состояние задаётся из control_inputs в stepOther()
    // после sand_system->step(), управление клавишами снято.

    // Контроллер машиниста — шаг для таймеров анимации вращения
    km[CAB1]->step(t, dt);
    km[CAB2]->step(t, dt);

    // Не читаем control_inputs во время процедуры автозапуска/автоостанова
    if (autoStartTimer->isStarted())
        return;

    // Автозапуск/автоостанов по сигналам от IOController
    for (auto cab_idx : {CAB1, CAB2})
    {
        bool autostart = control_inputs[cab_idx][CTRL_AUTOSTART_PROGRAM].toBool();
        if (autostart)
        {
            if (initAutostartProgram(cab_idx))
                autoStartTimer->start();

            initClientInputSignal(cab_idx, CTRL_AUTOSTART_PROGRAM, 0.0f);
            return;
        }

        bool autostop = control_inputs[cab_idx][CTRL_AUTOSTOP_PROGRAM].toBool();
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
        control_inputs[cab_idx][CTRL_AZV_CABINE_LIGHT].toBool()
            ? azv_cabine_light[cab_idx].set() : azv_cabine_light[cab_idx].reset();

        control_inputs[cab_idx][CTRL_AZV_PANEL_LIGHT].toBool()
            ? azv_panel_light[cab_idx].set() : azv_panel_light[cab_idx].reset();

        control_inputs[cab_idx][CTRL_AZV_COMMON_CONTROL].toBool()
            ? azv_common_control[cab_idx].set() : azv_common_control[cab_idx].reset();

        control_inputs[cab_idx][CTRL_AZV_UPR_TEPL].toBool()
            ? azv_upr_tepl[cab_idx].set() : azv_upr_tepl[cab_idx].reset();

        control_inputs[cab_idx][CTRL_AZV_FUEL_PUMP].toBool()
            ? azv_fuel_pump[cab_idx].set() : azv_fuel_pump[cab_idx].reset();

        control_inputs[cab_idx][CTRL_AZV_EDT_ON].toBool()
            ? azv_edt_on[cab_idx].set() : azv_edt_on[cab_idx].reset();

        control_inputs[cab_idx][CTRL_AZV_EPT_ON].toBool()
            ? azv_ept_on[cab_idx].set() : azv_ept_on[cab_idx].reset();

        control_inputs[cab_idx][CTRL_AZV_EPT_POWER].toBool()
            ? azv_ept_power[cab_idx].set() : azv_ept_power[cab_idx].reset();

        control_inputs[cab_idx][CTRL_AZV_SPOTLIGHT_LOW].toBool()
            ? azv_spotlight_low[cab_idx].set() : azv_spotlight_low[cab_idx].reset();

        control_inputs[cab_idx][CTRL_AZV_SPOTLIGHT_HIGH].toBool()
            ? azv_spotlight_high[cab_idx].set() : azv_spotlight_high[cab_idx].reset();

        // Тумблеры 3-поз (Switcher)
        tumbler_field_weak1[cab_idx].setPosition(
            control_inputs[cab_idx][CTRL_TUMBLER_FIELD_WEAK1].value);

        tumbler_field_weak2[cab_idx].setPosition(
            control_inputs[cab_idx][CTRL_TUMBLER_FIELD_WEAK2].value);

        tumbler_water_zaluzi[cab_idx].setPosition(
            control_inputs[cab_idx][CTRL_TUMBLER_WATER_ZALUZI].value);

        tumbler_oil_zaluzi[cab_idx].setPosition(
            control_inputs[cab_idx][CTRL_TUMBLER_OIL_ZALUZI].value);

        tumbler_bufferliht_L[cab_idx].setPosition(
            control_inputs[cab_idx][CTRL_TUMBLER_BUFFERLIGHT_L].value);

        tumbler_bufferliht_R[cab_idx].setPosition(
            control_inputs[cab_idx][CTRL_TUMBLER_BUFFERLIGHT_R].value);

        // Тумблеры (Toggle)
        control_inputs[cab_idx][CTRL_TUMBLER_VOLTMETER].toBool()
            ? tumbler_voltage[cab_idx].set() : tumbler_voltage[cab_idx].reset();

        control_inputs[cab_idx][CTRL_TUMBLER_DISEL_STOP].toBool()
            ? tumbler_disel_stop[cab_idx].set() : tumbler_disel_stop[cab_idx].reset();

        control_inputs[cab_idx][CTRL_TUMBLER_NUMBER_LIGHT].toBool()
            ? tumbler_number_light[cab_idx].set() : tumbler_number_light[cab_idx].reset();

        control_inputs[cab_idx][CTRL_TUMBLER_PANEL_POM_LIGHT].toBool()
            ? tumbler_panel_pom_light[cab_idx].set() : tumbler_panel_pom_light[cab_idx].reset();

        control_inputs[cab_idx][CTRL_TUMBLER_RESERVE_LIGHT].toBool()
            ? tumbler_reserve_light[cab_idx].set() : tumbler_reserve_light[cab_idx].reset();

        control_inputs[cab_idx][CTRL_TUMBLER_CAB_HEATER].toBool()
            ? tumbler_cab_heater[cab_idx].set() : tumbler_cab_heater[cab_idx].reset();

        control_inputs[cab_idx][CTRL_TUMBLER_CAB_FAN].toBool()
            ? tumbler_cab_fan[cab_idx].set() : tumbler_cab_fan[cab_idx].reset();

        // Тумблер ПСС (декоративный)
        control_inputs[cab_idx][CTRL_TUMBLER_PSS].toBool()
            ? tumbler_pss[cab_idx].set() : tumbler_pss[cab_idx].reset();

        // Кнопки
        control_inputs[cab_idx][CTRL_BUTTON_DISEL_START].toBool()
            ? button_disel_start[cab_idx].set() : button_disel_start[cab_idx].reset();

        control_inputs[cab_idx][CTRL_BUTTON_BRAKE_RELEASE].toBool()
            ? button_brake_release[cab_idx].set() : button_brake_release[cab_idx].reset();

        control_inputs[cab_idx][CTRL_BUTTON_SVISTOK].toBool()
            ? button_svistok[cab_idx].set() : button_svistok[cab_idx].reset();

        control_inputs[cab_idx][CTRL_BUTTON_SVISTOK_POM].toBool()
            ? button_svistok_pom[cab_idx].set() : button_svistok_pom[cab_idx].reset();

        control_inputs[cab_idx][CTRL_BUTTON_TIFON].toBool()
            ? button_tifon[cab_idx].set() : button_tifon[cab_idx].reset();

        control_inputs[cab_idx][CTRL_BUTTON_TIFON_POM].toBool()
            ? button_tifon_pom[cab_idx].set() : button_tifon_pom[cab_idx].reset();

        control_inputs[cab_idx][CTRL_SAND_BUTTON].toBool()
            ? button_sand[cab_idx].set() : button_sand[cab_idx].reset();

        // РБ
        control_inputs[cab_idx][CTRL_RBS].toBool()
            ? rb[cab_idx][RBS].set() : rb[cab_idx][RBS].reset();

        control_inputs[cab_idx][CTRL_RB].toBool()
            ? rb[cab_idx][RB].set() : rb[cab_idx][RB].reset();

        control_inputs[cab_idx][CTRL_RBP].toBool()
            ? rb[cab_idx][RBP].set() : rb[cab_idx][RBP].reset();

        // Переключатели
        brake_force_switch[cab_idx].setPosition(
            control_inputs[cab_idx][CTRL_BRAKE_FORCE_SWITCH].value);

        // Автоведение, маневровый режим
        control_inputs[cab_idx][CTRL_AUTOPILOT].toBool()
            ? autopilot_switcher[cab_idx].set() : autopilot_switcher[cab_idx].reset();

        control_inputs[cab_idx][CTRL_SHUNTING_MODE].toBool()
            ? shunting_mode_switcher[cab_idx].set() : shunting_mode_switcher[cab_idx].reset();

        // ЭПК
        control_inputs[cab_idx][CTRL_EPK_INSERTION].toBool()
            ? epk[cab_idx]->insertKey(true) : epk[cab_idx]->insertKey(false);

        control_inputs[cab_idx][CTRL_KEY_EPK].toBool()
            ? epk[cab_idx]->setKeyOn(true) : epk[cab_idx]->setKeyOn(false);

        // Реверсор, штурвал КМ и тормозные краны.
        // При активном автоведении клиентское управление не применяем,
        // а отдаём клиенту фактическое состояние органов.
        if (!autopilot_switcher[cab_idx].getState())
        {
            // Реверсор и штурвал КМ
            bool revers_insert = control_inputs[cab_idx][CTRL_REVERS_INSERTION].toBool();
            km[cab_idx]->insertReversHandle(revers_insert);

            int revers_pos = control_inputs[cab_idx][CTRL_REVERS_POSITION].value;
            km[cab_idx]->setReversHandlePos(revers_pos);

            int main_pos = control_inputs[cab_idx][CTRL_KM_MAIN_POSITION].value;
            km[cab_idx]->setPosition(main_pos);

            // Тормозные краны
            int krm_pos = control_inputs[cab_idx][CTRL_KRM_395].value;
            brake_crane[cab_idx]->setHandlePosition(krm_pos);

            double kvt_pos = control_inputs[cab_idx][CTRL_KVT_254].value;
            loco_crane[cab_idx]->setHandlePosition(kvt_pos);
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
        bool is_lock367_insert = control_inputs[cab_idx][CTRL_LOCK_367_INSERTION].toBool();
        brake_lock[cab_idx]->insertLockHandle(is_lock367_insert);
        brake_lock[cab_idx]->setStateOn(is_lock367_insert);

        brake_lock[cab_idx]->setCombineCranePosition(
            control_inputs[cab_idx][CTRL_COMBINE_KRAN].value);
    }

    // Общие сигналы (shared)
    auto &shared_inputs = control_inputs[CABS_NUM];

    // Расцепные рычаги: фиксация в расцепленном положении либо аналоговое положение
    if (shared_inputs[CTRL_OPER_ROD_FIX_FWD].toBool())
        oper_rod_fwd->setExternalState(-1.0);
    else
        oper_rod_fwd->setExternalState(shared_inputs[CTRL_OPER_ROD_FWD].value);

    if (shared_inputs[CTRL_OPER_ROD_FIX_BWD].toBool())
        oper_rod_bwd->setExternalState(-1.0);
    else
        oper_rod_bwd->setExternalState(shared_inputs[CTRL_OPER_ROD_BWD].value);

    bool ac_bc_fwd = shared_inputs[CTRL_ANGLECOCK_BC_FWD].toBool();
    ac_bc_fwd ? anglecock_bc_fwd->open() : anglecock_bc_fwd->close();

    bool ac_bc_bwd = shared_inputs[CTRL_ANGLECOCK_BC_BWD].toBool();
    ac_bc_bwd ? anglecock_bc_bwd->open() : anglecock_bc_bwd->close();

    // Концевые краны питательной магистрали
    bool ac_fl_fwd = shared_inputs[CTRL_ANGLECOCK_FL_FWD].toBool();
    ac_fl_fwd ? anglecock_fl_fwd->open() : anglecock_fl_fwd->close();

    bool ac_fl_bwd = shared_inputs[CTRL_ANGLECOCK_FL_BWD].toBool();
    ac_fl_bwd ? anglecock_fl_bwd->open() : anglecock_fl_bwd->close();

    // Концевые краны тормозной магистрали
    bool ac_bp_fwd = shared_inputs[CTRL_ANGLECOCK_BP_FWD].toBool();
    ac_bp_fwd ? anglecock_bp_fwd->open() : anglecock_bp_fwd->close();

    bool ac_bp_bwd = shared_inputs[CTRL_ANGLECOCK_BP_BWD].toBool();
    ac_bp_bwd ? anglecock_bp_bwd->open() : anglecock_bp_bwd->close();

    // Рукава магистралей: 0 - разъединён, 2 - соединён (1 - нейтраль)
    hose_bc_fwd->setExternalState(shared_inputs[CTRL_HOSE_BC_FWD].toBool() ? 2 : 0);
    hose_bc_bwd->setExternalState(shared_inputs[CTRL_HOSE_BC_BWD].toBool() ? 2 : 0);
    hose_fl_fwd->setExternalState(shared_inputs[CTRL_HOSE_FL_FWD].toBool() ? 2 : 0);
    hose_fl_bwd->setExternalState(shared_inputs[CTRL_HOSE_FL_BWD].toBool() ? 2 : 0);
    hose_bp_fwd->setExternalState(shared_inputs[CTRL_HOSE_BP_FWD].toBool() ? 2 : 0);
    hose_bp_bwd->setExternalState(shared_inputs[CTRL_HOSE_BP_BWD].toBool() ? 2 : 0);

    // АЗВ "Компрессор" (общий)
    bool azv_motor_comp_on = shared_inputs[CTRL_AZV_MOTOR_COMPRESSOR].toBool();
    azv_motor_comp_on ? azv_motor_compressor.set() : azv_motor_compressor.reset();

    // АЗВ ВВК (общие)
    shared_inputs[CTRL_AZV_BUFFER_LIGHTS].toBool()
        ? azv_buffer_lights.set() : azv_buffer_lights.reset();

    shared_inputs[CTRL_AZV_CONTROL_CIRCUIT].toBool()
        ? azv_control_circuit.set() : azv_control_circuit.reset();

    shared_inputs[CTRL_AZV_DISEL_LIGHT].toBool()
        ? azv_disel_light.set() : azv_disel_light.reset();

    shared_inputs[CTRL_AZV_EPK_POWER_MINUS].toBool()
        ? azv_epk_power_minus.set() : azv_epk_power_minus.reset();

    shared_inputs[CTRL_AZV_EPK_POWER_PLUS].toBool()
        ? azv_epk_power_plus.set() : azv_epk_power_plus.reset();

    shared_inputs[CTRL_AZV_FIRE_ALARM].toBool()
        ? azv_fire_alarm.set() : azv_fire_alarm.reset();

    shared_inputs[CTRL_AZV_FUEL_PUMP_VVK].toBool()
        ? azv_fuel_pump_vvk.set() : azv_fuel_pump_vvk.reset();

    shared_inputs[CTRL_AZV_HEATER].toBool()
        ? azv_heater.set() : azv_heater.reset();

    shared_inputs[CTRL_AZV_SOCKETS_POWER].toBool()
        ? azv_sockets_power.set() : azv_sockets_power.reset();

    shared_inputs[CTRL_AZV_SOCKETS_POWER_2].toBool()
        ? azv_sockets_power_2.set() : azv_sockets_power_2.reset();

    shared_inputs[CTRL_AZV_TROLLEYS_LIGHT].toBool()
        ? azv_trolleys_light.set() : azv_trolleys_light.reset();

    shared_inputs[CTRL_AZV_VVK_LIGHT].toBool()
        ? azv_vvk_light.set() : azv_vvk_light.reset();

    shared_inputs[CTRL_AZV_WATCH_DISEL_LIGHT].toBool()
        ? azv_watch_disel_light.set() : azv_watch_disel_light.reset();

    // Органы панели ВВК
    shared_inputs[CTRL_VVK_EDT_TUMBLER].toBool()
        ? edt_tumbler.set() : edt_tumbler.reset();

    shared_inputs[CTRL_VVK_SHUNTING_TUMBLER].toBool()
        ? shunting_tumbler.set() : shunting_tumbler.reset();

    shared_inputs[CTRL_VVK_MANUAL_FUEL_PUMP].toBool()
        ? manual_fuel_pumping.set() : manual_fuel_pumping.reset();

    shared_inputs[CTRL_VVK_RESERVED_FUEL_PUMP].toBool()
        ? reserved_fuel_pump.set() : reserved_fuel_pump.reset();

    shared_inputs[CTRL_VVK_AUTOSTOP_TUMBLER].toBool()
        ? autostop_tumbler.set() : autostop_tumbler.reset();

    shared_inputs[CTRL_VVK_ISOLATION_CHECK].toBool()
        ? isolation_check.set() : isolation_check.reset();

    found_ground_switch.setNumPositions(21);
    found_ground_switch.setPosition(shared_inputs[CTRL_VVK_FOUND_GROUND_SWITCH].value);

    shared_inputs[CTRL_VVK_GROUND_RELAY_BUTTON].toBool()
        ? ground_relay_button.set() : ground_relay_button.reset();

    shared_inputs[CTRL_VVK_KEEP_FUEL_PUMP_BUTTON].toBool()
        ? keep_fuel_pump_button.set() : keep_fuel_pump_button.reset();

    shared_inputs[CTRL_VVK_V_MINUS_BUTTON].toBool()
        ? v_minus_button.set() : v_minus_button.reset();

    shared_inputs[CTRL_VVK_V_PLUS_BUTTON].toBool()
        ? v_plus_button.set() : v_plus_button.reset();

    // Тумблеры ОМ
    for (size_t i = 0; i < NUM_MOTORS; ++i)
    {
        shared_inputs[CTRL_TUMBLER_OM1 + i].toBool()
            ? tumbler_om[i].set() : tumbler_om[i].reset();
    }

    // Переключатели ВВК
    for (size_t i = 0; i < VVK_SW_NUM; ++i)
    {
        shared_inputs[CTRL_VVK_PA1 + i].toBool()
            ? vvk_sw[i].set() : vvk_sw[i].reset();
    }

    // Тумблер числа тепловозов
    shared_inputs[CTRL_TUMBLER_LOCO_COUNT].toBool()
        ? tumbler_loco_count.set() : tumbler_loco_count.reset();

    // Рубильник АКБ
    shared_inputs[CTRL_ACCUM_SWITCH].toBool()
        ? accum_switch.set() : accum_switch.reset();

    // Ручки дверей кабин
    shared_inputs[CTRL_CABINE1_DOOR_HANDLE].toBool()
        ? cabine1_door_handle.set() : cabine1_door_handle.reset();

    shared_inputs[CTRL_CABINE2_DOOR_HANDLE].toBool()
        ? cabine2_door_handle.set() : cabine2_door_handle.reset();

    for (auto cab_idx : {CAB1, CAB2})
    {
        uint16_t d = (SPOTLIGHT_BWD - SPOTLIGHT_FWD) * cab_idx;

        // Тумблер отопления
        control_inputs[cab_idx][CTRL_CAB1_TUMBLER_HEATER + cab_idx].toBool()
            ? tumbler_heater[cab_idx].set() : tumbler_heater[cab_idx].reset();

        // АЗВ вентиляции
        control_inputs[cab_idx][CTRL_CAB1_AZV_VENT + cab_idx].toBool()
            ? azv_ventilation[cab_idx].set() : azv_ventilation[cab_idx].reset();

        // Тумблер освещения кабины (собственный)
        control_inputs[cab_idx][CTRL_CAB1_TUMBLER_CABINE_LIGHT + cab_idx].toBool()
            ? tumbler_cabine_light[cab_idx].set() : tumbler_cabine_light[cab_idx].reset();
    }
}