#include    <tep70.h>
#include    <tep70-controls.h>

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

    // Переключатель кабины
    if (getKeyState(KEY_Minus))
        cabine_switcher->setCabineIndex(CAB1);
    else if (getKeyState(KEY_Equals))
        cabine_switcher->setCabineIndex(CAB2);
    cabine_switcher->step(t, dt);

    // Песочница — состояние задаётся из control_inputs в stepOther()
    // после sand_system->step(), управление клавишами снято.
    horn->setControl(&pressed_keys);

    // АЗВ компрессор — пока не переведён, шагаем вручную
    azv_motor_compressor.step();

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

        // Управление блокировкой УБТ-367
        bool is_lock367_insert = control_inputs[cab_idx][CTRL_LOCK_367_INSERTION].toBool();
        brake_lock[cab_idx]->insertLockHandle(is_lock367_insert);
        brake_lock[cab_idx]->setStateOn(is_lock367_insert);

        brake_lock[cab_idx]->setCombineCranePosition(
            control_inputs[cab_idx][CTRL_COMBINE_KRAN].value);
    }

    // Общие сигналы (shared)
    auto &shared_inputs = control_inputs[CABS_NUM];

    bool rod_fwd = shared_inputs[CTRL_OPER_ROD_FWD].toBool();
    oper_rod_fwd->setExternalState(rod_fwd ? -1.0 : 1.0);

    bool rod_bwd = shared_inputs[CTRL_OPER_ROD_BWD].toBool();
    oper_rod_bwd->setExternalState(rod_bwd ? -1.0 : 1.0);

    bool ac_bc_fwd = shared_inputs[CTRL_ANGLECOCK_BC_FWD].toBool();
    ac_bc_fwd ? anglecock_bc_fwd->open() : anglecock_bc_fwd->close();

    bool ac_bc_bwd = shared_inputs[CTRL_ANGLECOCK_BC_BWD].toBool();
    ac_bc_bwd ? anglecock_bc_bwd->open() : anglecock_bc_bwd->close();
}