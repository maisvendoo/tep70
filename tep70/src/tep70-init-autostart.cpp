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

    bool is_requested_cab_active = (cab_autostart_request == CAB1) ? cabine_switcher->isCabine1()
                                                                   : cabine_switcher->isCabine2();

    if (!is_requested_cab_active)
        return false;

    if (km[(cab_autostart_request == CAB1) ? CAB2 : CAB1]->isReversHandle())
        return false;

    if (!brake_lock[cab_autostart_request]->isLockHandleAllowed())
        return false;

    if (!epk[cab_autostart_request]->isKeyAllowed())
        return false;

    autostart_cab = cab_autostart_request;
    autostart_mode = AS_MODE_START;

    autostart_saved_state[autostart_cab].field_weak1_pos = tumbler_field_weak1[autostart_cab].getPosition();
    autostart_saved_state[autostart_cab].field_weak2_pos = tumbler_field_weak2[autostart_cab].getPosition();
    autostart_saved_state[autostart_cab].km_main_shaft = km[autostart_cab]->getPositionNumber();
    autostart_saved_state[autostart_cab].km_revers = km[autostart_cab]->getReversHandlePos();
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
    if (autostart_mode == AS_MODE_STOP)
    {
        slotAutostop();
        return;
    }

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

        km[autostart_cab]->setReversHandlePos(1);

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

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
bool TEP70::initAutostopProgram(int cab_autostop_request)
{
    if (autoStartTimer->isStarted())
        return false;

    if ((cab_autostop_request != CAB1) && (cab_autostop_request != CAB2))
        return false;

    bool is_requested_cab_active = (cab_autostop_request == CAB1) ? cabine_switcher->isCabine1()
                                                                  : cabine_switcher->isCabine2();

    if (!is_requested_cab_active)
        return false;

    autostart_cab = cab_autostop_request;
    autostart_mode = AS_MODE_STOP;

    // Выключаем автоведение, если оно было включено
    OffAutopilot();

    // Переводим органы управления в программное управление
    km[CAB1]->setControl();
    km[CAB2]->setControl();
    brake_lock[CAB1]->setControl();
    brake_lock[CAB2]->setControl();
    epk[CAB1]->setControl();
    epk[CAB2]->setControl();

    // Возвращаем главный вал КМ в нулевое положение.
    // Это необходимо для работы цепи прокачки масла после остановки
    // дизеля и для снятия блокировки реверсивного вала
    km[autostart_cab]->setPosition(0);

    // Восстанавливаем состояние органов управления, сохранённое перед автостартом
    if (autostart_state_saved[autostart_cab])
    {
        tumbler_field_weak1[autostart_cab].setPosition(autostart_saved_state[autostart_cab].field_weak1_pos);
        tumbler_field_weak2[autostart_cab].setPosition(autostart_saved_state[autostart_cab].field_weak2_pos);

        km[autostart_cab]->setReversHandlePos(0);

        brake_lock[autostart_cab]->setStateOn(autostart_saved_state[autostart_cab].brake_lock_on);
    }

    // Выключаем ключ ЭПК (возврат в исходное положение перед автостартом).
    // РБС не нажимаем: контроль бдительности нужен только при включённом ЭПК
    epk[autostart_cab]->setKeyOn(false);

    // Отключаем органы управления в последовательности, обратной автозапуску
    start_count = 0;
    autostart_triggers.clear();

    autostart_triggers.push_back(&azv_upr_tepl[autostart_cab]);
    autostart_triggers.push_back(&azv_fuel_pump[autostart_cab]);
    autostart_triggers.push_back(&azv_common_control[autostart_cab]);

    stop_diesel_was_running = false;
    stop_recirculation_seen = false;
    stop_wait_ticks = 0;

    return true;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
bool TEP70::isOilRecirculationFinished()
{
    // Прокачка идёт (контактор маслопрокачивающего насоса включён) — ждём завершения
    if (kontaktor_oil_pump->getContactState(0))
    {
        stop_recirculation_seen = true;
        return false;
    }

    // КМН выключен
    if (stop_recirculation_seen)
        return true;

    // Прокачка не наблюдалась
    if (stop_diesel_was_running)
        return false;   // дизель ещё останавливается — ждём начала прокачки

    // Дизель уже был остановлен — прокачки не будет
    return true;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void TEP70::slotAutostop()
{
    if (start_count < autostart_triggers.size())
    {
        // Перед выключением АВ "Управление общее" ожидаем завершения
        // минутной прокачки масла после остановки дизеля
        if (autostart_triggers[start_count] == &azv_common_control[autostart_cab])
        {
            if (!isOilRecirculationFinished())
            {
                stop_wait_ticks++;

                // Страховка от зависания процедуры
                if (stop_wait_ticks > 360)
                {
                    stop_recirculation_seen = true;
                }
                else
                {
                    return;
                }
            }
        }

        // Фиксируем, работал ли дизель в момент выключения АВ "Топливный насос"
        if (autostart_triggers[start_count] == &azv_fuel_pump[autostart_cab])
        {
            stop_diesel_was_running = static_cast<bool>(ru6->getContactState(0));
        }

        autostart_triggers[start_count]->reset();

        start_count++;
    }
    else
    {
        autoStartTimer->stop();
        start_count = 0;

        km[CAB1]->setControl(&pressed_keys_by_cabine[CAB1]);
        km[CAB2]->setControl(&pressed_keys_by_cabine[CAB2]);
        brake_lock[CAB1]->setControl(&pressed_keys_by_cabine[CAB1]);
        brake_lock[CAB2]->setControl(&pressed_keys_by_cabine[CAB2]);
        epk[CAB1]->setControl(&pressed_keys_by_cabine[CAB1]);
        epk[CAB2]->setControl(&pressed_keys_by_cabine[CAB2]);
    }
}
