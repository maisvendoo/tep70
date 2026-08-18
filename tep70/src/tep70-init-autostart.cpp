#include    <tep70.h>

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
bool TEP70::initAutostartProgram(int cab_autostart_request)
{
    if (autoStartTimer->isStarted())
        return false;

    if ((cab_autostart_request != CAB1) && (cab_autostart_request != CAB2))
        return false;

    if (km[(cab_autostart_request == CAB1) ? CAB2 : CAB1]->isReversHandle())
        return false;

    if (!brake_lock[cab_autostart_request]->isLockHandleAllowed())
        return false;

    if (!epk[cab_autostart_request]->isKeyAllowed())
        return false;

    autostart_cab = cab_autostart_request;

    autostart_saved_state[autostart_cab].field_weak1_pos = tumbler_field_weak1[autostart_cab].getPosition();
    autostart_saved_state[autostart_cab].field_weak2_pos = tumbler_field_weak2[autostart_cab].getPosition();
    autostart_saved_state[autostart_cab].km_main_shaft = km[autostart_cab]->getPositionNumber();
    autostart_saved_state[autostart_cab].km_revers = km[autostart_cab]->getReversState();
    autostart_saved_state[autostart_cab].brake_lock_on = brake_lock[autostart_cab]->isStateOn();
    autostart_state_saved[autostart_cab] = true;

    km[autostart_cab]->insertReversHandle(true);
    brake_lock[autostart_cab]->setStateOn(true);
    epk[autostart_cab]->insertKey(true);

    km[CAB1]->setControl();
    km[CAB2]->setControl();
    brake_lock[CAB1]->setControl();
    brake_lock[CAB2]->setControl();
    epk[CAB1]->setControl();
    epk[CAB2]->setControl();

    start_count = 0;
    autostart_triggers.clear();

    autostart_triggers.push_back(&azv_common_control[autostart_cab]);
    autostart_triggers.push_back(&azv_fuel_pump[autostart_cab]);
    autostart_triggers.push_back(&button_disel_start[autostart_cab]);
    autostart_triggers.push_back(&azv_upr_tepl[autostart_cab]);

    if (!epk[autostart_cab]->isKeyOn())
    {
        autostart_triggers.push_back(&rb[autostart_cab][RBS]);
    }

    return true;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void TEP70::slotAutostart()
{
    if (start_count < autostart_triggers.size())
    {
        // Отжимаем уже нажатую кнопку пуска дизеля
        if (button_disel_start[autostart_cab].getState())
        {
            button_disel_start[autostart_cab].reset();
        }

        // Ожидаем полный запуск дизеля
        if (autostart_triggers[start_count] == &azv_upr_tepl[autostart_cab])
        {
            bool is_battery_charge = static_cast<bool>(hs_p(100.0 - starter_generator->getVoltage()));

            if (is_battery_charge)
            {
                return;
            }
        }

        if ((autostart_triggers[start_count] == &rb[autostart_cab][RBS]) &&
            !epk[autostart_cab]->isKeyOn())
        {
            epk[autostart_cab]->setKeyOn(true);
            return;
        }

        autostart_triggers[start_count++]->set();
    }
    else
    {
        autoStartTimer->stop();
        start_count = 0;

        tumbler_field_weak1[autostart_cab].setPosition(2);
        tumbler_field_weak2[autostart_cab].setPosition(2);

        km[autostart_cab]->setReversFwd();

        km[CAB1]->setControl(&pressed_keys_by_cabine[CAB1]);
        km[CAB2]->setControl(&pressed_keys_by_cabine[CAB2]);
        brake_lock[CAB1]->setControl(&pressed_keys_by_cabine[CAB1]);
        brake_lock[CAB2]->setControl(&pressed_keys_by_cabine[CAB2]);
        epk[CAB1]->setControl(&pressed_keys_by_cabine[CAB1]);
        epk[CAB2]->setControl(&pressed_keys_by_cabine[CAB2]);

        if (auto_start_autopilot)
        {
            autopilot_switcher[CAB1].set();
            autopilot_switcher[CAB2].set();
        }
    }
}
