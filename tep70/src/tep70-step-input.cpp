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

    // Песочница, тифон, свисток
    sand_system->setControl(&pressed_keys);
    horn->setControl(&pressed_keys);

    // АЗВ компрессор
    azv_motor_compressor.step();

    // Шаг переключателей, пока не переведённых на IOController
    for (auto cab_idx : {CAB1, CAB2})
    {
        brake_lock[cab_idx]->step(t, dt);
        km[cab_idx]->step(t, dt);

        azv_cabine_light[cab_idx].step();
        azv_panel_light[cab_idx].step();
        azv_edt_on[cab_idx].step();
        azv_ept_on[cab_idx].step();
        azv_ept_power[cab_idx].step();
        azv_spotlight_low[cab_idx].step();
        azv_spotlight_high[cab_idx].step();

        tumbler_field_weak1[cab_idx].step();
        tumbler_field_weak2[cab_idx].step();
        tumbler_water_zaluzi[cab_idx].step();
        tumbler_oil_zaluzi[cab_idx].step();

        tumbler_voltage[cab_idx].step();
        tumbler_disel_stop[cab_idx].step();
        tumbler_bufferliht_L[cab_idx].step();
        tumbler_bufferliht_R[cab_idx].step();
        tumbler_number_light[cab_idx].step();
        

        button_brake_release[cab_idx].step();
        button_svistok[cab_idx].step();
        button_tifon[cab_idx].step();

        rb[cab_idx][RBS].step();
        rb[cab_idx][RB].step();
        rb[cab_idx][RBP].step();

        brake_force_switch[cab_idx].step();
        autopilot_switcher[cab_idx].step();
        shunting_mode_switcher[cab_idx].step();
    }

    // Не читаем control_inputs во время процедуры автозапуска/автоостанова
    if (autoStartTimer->isStarted())
        return;

    // Автозапуск/автоостанов по сигналам от IOController
    for (auto cab_idx : {CAB1, CAB2})
    {
        if (control_inputs[cab_idx][CTRL_AUTOSTART_PROGRAM].toBool())
        {
            if (initAutostartProgram(cab_idx))
                autoStartTimer->start();

            initClientInputSignal(cab_idx, CTRL_AUTOSTART_PROGRAM, 0.0f);
            return;
        }
    }

    for (auto cab_idx : {CAB1, CAB2})
    {
        if (control_inputs[cab_idx][CTRL_AUTOSTOP_PROGRAM].toBool())
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
        // АЗВ "Управление общее"
        control_inputs[cab_idx][CTRL_AZV_COMMON_CONTROL].toBool()
            ? azv_common_control[cab_idx].set()
            : azv_common_control[cab_idx].reset();

        // АЗВ "Топливный насос"
        control_inputs[cab_idx][CTRL_AZV_FUEL_PUMP].toBool()
            ? azv_fuel_pump[cab_idx].set()
            : azv_fuel_pump[cab_idx].reset();

        // Кнопка "Пуск дизеля"
        control_inputs[cab_idx][CTRL_BUTTON_DISEL_START].toBool()
            ? button_disel_start[cab_idx].set()
            : button_disel_start[cab_idx].reset();
    }
}