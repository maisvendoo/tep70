#include    "tep70bs.h"

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void TEP70BS::keyProcess(const simulator_time_t& t, const double& dt)
{
    if (autoStartTimer.isStarted())
        return;
/*
    // Переключатель-блокировка рабочей кабины
    if (getKeyState(KEY_Minus))
        cabine_switcher->setCabineIndex(CAB1);
    else if (getKeyState(KEY_Equals))
        cabine_switcher->setCabineIndex(CAB2);
    cabine_switcher->step(t.simulation_seconds, dt);*/
    cabine_switcher->setCabineIndex(CAB1);

    // Песочница
    sand_system->setControl(&pressed_keys);
    // Тифон, свисток
    horn->setControl(&pressed_keys);

    // Не допускаем двух рукояток в устройствах блокировки тормозов
    brake_lock[CAB2]->allowLockHandle(!(brake_lock[CAB1]->isLockHandle()));
    brake_lock[CAB1]->allowLockHandle(!(brake_lock[CAB2]->isLockHandle()));

    // Не допускаем двух ключей в электропневматических клапанах автостопа
    epk[CAB2]->allowKey(!(epk[CAB1]->isKey()));
    epk[CAB1]->allowKey(!(epk[CAB2]->isKey()));

    for (auto cab_idx : {CAB1, CAB2})
    {
        // Управляем краном, учитывая возможное наличие внешнего пульта
        // TODO // перенести freejoy во вьювер, его команды передавать по сети,
        // TODO // и также указывая индекс кабины
        if (control_signals.analogSignal[CS_BRAKE_CRANE].is_active)
        {
            int brake_crane_pos = static_cast<int>(control_signals.analogSignal[CS_BRAKE_CRANE].cur_value);
            brake_crane[cab_idx]->setHandlePosition(brake_crane_pos);
        }
        else
        {
            brake_crane[cab_idx]->setControl(&pressed_keys_by_cabine[cab_idx]);
        }

        // Управляем краном, учитывая возможное наличие внешнего пульта
        // TODO // перенести freejoy во вьювер, его команды передавать по сети,
        // TODO // и также указывая индекс кабины
        if (control_signals.analogSignal[CS_LOCO_CRANE].is_active)
        {
            double pos = 0.0;

            if (static_cast<bool>(control_signals.analogSignal[CS_RELEASE_VALVE].cur_value))
            {
                loco_crane[cab_idx]->release(true);
                pos = -1.0;
            }
            else
            {
                loco_crane[cab_idx]->release(false);
                pos = control_signals.analogSignal[CS_LOCO_CRANE].cur_value;
            }

            loco_crane[cab_idx]->setHandlePosition(pos);
        }
        else
        {
            loco_crane[cab_idx]->setControl(&pressed_keys_by_cabine[cab_idx]);
        }

        // Контроллер машиниста
        km[cab_idx]->setControl(&pressed_keys_by_cabine[cab_idx]);

        // Тумблеры, кнопки
        azv_common_control[cab_idx].step();
        azv_upr_tepl[cab_idx].step();
        azv_fuel_pump[cab_idx].step();
        azv_edt_on[cab_idx].step();
        azv_edt_power[cab_idx].step();
        azv_ept_on[cab_idx].step();
        azv_motor_compressor[cab_idx].step();
        tumbler_voltage[cab_idx].step();
        tumbler_disel_stop[cab_idx].step();
        tumbler_field_weak1[cab_idx].step();
        tumbler_field_weak2[cab_idx].step();
        tumbler_water_zaluzi[cab_idx].step();
        tumbler_oil_zaluzi[cab_idx].step();
        brake_force_switch[cab_idx].step();
        button_disel_start[cab_idx].step();
        button_brake_release[cab_idx].step();
        button_svistok[cab_idx].step();
        button_tifon[cab_idx].step();

        // Нажатие РБС
        // Если активна РБС на внешнем пульте
        // TODO // перенести freejoy во вьювер, его команды передавать по сети,
        // TODO // и также указывая индекс кабины
        if (control_signals.analogSignal[CS_RBS].is_active)
        {
            // реагируем на состояние РБС на внешнем пульте
            if (static_cast<bool>(control_signals.analogSignal[CS_RBS].cur_value))
                rb[cab_idx][RBS].set();
            else
                rb[cab_idx][RBS].reset();
        }
        else // иначе
        {
            // обрабатываем клавиши
            rb[cab_idx][RBS].step();
        }
        rb[cab_idx][RB].step();
        rb[cab_idx][RBP].step();
    }
}
