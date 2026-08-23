#include    "km-2202.h"

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
ControllerKM2202::ControllerKM2202(QObject *parent) : Device(parent)
{
    main_shaft_timer.firstProcess(true);
    connect(&main_shaft_timer, &Timer::process,
            this, &ControllerKM2202::slotRotateMainShaft);

    revers_shaft_timer.firstProcess(true);
    connect(&revers_shaft_timer, &Timer::process,
            this, &ControllerKM2202::slotRotateReversShaft);
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
ControllerKM2202::~ControllerKM2202()
{

}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void ControllerKM2202::allowReversHandle(bool allow)
{
    is_reverse_handle_allowed = allow;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
bool ControllerKM2202::isReversHandleAllowed() const
{
    return is_reverse_handle_allowed;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void ControllerKM2202::insertReversHandle(bool insert)
{
    insert = insert && is_reverse_handle_allowed;

    if (insert)
    {
        // Вставляем реверсивную рукоятку
        is_revers_handle.set();
        return;
    }

    // Извлечение реверсивной рукоятки только в нулевом положении
    if (rs_position == RS_ZERO)
    {
        is_revers_handle.reset();
    }
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
bool ControllerKM2202::isReversHandle() const
{
    return is_revers_handle.getState();
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void ControllerKM2202::setReversHandlePos(int8_t pos)
{
    if (isReversHandle())
    {
        if (ms_position == MS_ZERO)
        {
            // Задаём новое положение реверсивной рукоятки
            pos = std::clamp(pos, static_cast<std::int8_t>(RS_BACKWARD), static_cast<std::int8_t>(RS_FORWARD));
        }
        else
        {
            // Если штурвал не в нулевой позиции, реверсивка заблокирована в своём положении
            pos = rs_position;
        }
    }
    else
    {
        // При снятой реверсивке всегда в нуле
        rs_position = RS_ZERO;
    }

    if (rs_position == pos)
    {
        // Переключать нечего, выходим
        return;
    }

    // Переключаем и озвучиваем
    rs_position = pos;
    sound_states[REVERS_SHAFT].play();
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
int8_t ControllerKM2202::getReversHandlePos() const
{
    return rs_position;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void ControllerKM2202::setPosition(std::int8_t pos)
{
    if (rs_position == RS_ZERO)
    {
        // Блокировка поворота штурвала при нулевом положении реверсивки
        pos = MS_ZERO;
    }
    else
    {
        pos = std::clamp(pos, static_cast<std::int8_t>(MS_ZERO), static_cast<std::int8_t>(MS_MAX_POSITION));
    }


    if (ms_position == pos)
    {
        // Переключать нечего, выходим
        return;
    }

    // Переключаем и озвучиваем
    ms_position = pos;
    sound_states[MAIN_SHAFT].play();
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
float ControllerKM2202::getMainShaftPos() const
{
    return static_cast<float>(ms_position) / MS_MAX_POSITION;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
sound_state_t ControllerKM2202::getSoundState(size_t idx) const
{
    if (idx < NUM_SOUNDS)
    {
        return sound_states[idx];
    }

    return is_revers_handle.getSoundState(idx - NUM_SOUNDS);
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
float ControllerKM2202::getSoundSignal(size_t idx) const
{
    if (idx < NUM_SOUNDS)
    {
        return sound_states[idx].createSoundSignal();
    }

    return is_revers_handle.getSoundSignal(idx - NUM_SOUNDS);
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void ControllerKM2202::preStep(state_vector_t &Y, double t)
{
    Q_UNUSED(Y)
    Q_UNUSED(t)
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void ControllerKM2202::ode_system(const state_vector_t &Y,
                                  state_vector_t &dYdt,
                                  double t)
{
    Q_UNUSED(Y)
    Q_UNUSED(dYdt)
    Q_UNUSED(t)
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void ControllerKM2202::load_config(CfgReader &cfg)
{
    QString secName = "Device";

    int timeout = 200;
    if (cfg.getInt(secName, "MainShaftDelay", timeout))
    {
        ms_delay = static_cast<double>(timeout) / 1000.0;
    }

    main_shaft_timer.setTimeout(ms_delay);

    timeout = 200;
    if (cfg.getInt(secName, "ReversShaftDelay", timeout))
    {
        rs_delay = static_cast<double>(timeout) / 1000.0;
    }

    revers_shaft_timer.setTimeout(rs_delay);

    QDomNode secNode = cfg.getFirstSection("Pos");
    while (!secNode.isNull())
    {
        int number = 0;
        cfg.getInt(secNode, "Number", number);

        double shaft_freq = 0;
        cfg.getDouble(secNode, "ShaftFreq", shaft_freq);

        n_ref.insert(static_cast<std::int8_t>(number), shaft_freq);

        secNode = cfg.getNextSection();
    }
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void ControllerKM2202::stepKeysControl(double t, double dt)
{
    if (getKeyState(pressed_keys, KEY_A) || getKeyState(pressed_keys, KEY_D) )
    {
        if (getKeyState(*pressed_keys, KEY_A))
        {
            ms_dir = 1;
            main_shaft_timer.start();
        }
        else if (getKeyState(*pressed_keys, KEY_D))
        {
/*            if (!isModifier(*pressed_keys, MODIFIER_Control))
            {*/
                ms_dir = -1;
                main_shaft_timer.start();
/*            }
            else
            {
                setPosition(MS_ZERO);
            }*/
        }
    }
    else
    {
        main_shaft_timer.stop();
    }


    if (getKeyState(pressed_keys, KEY_W) || getKeyState(pressed_keys, KEY_S))
    {
        if (getKeyState(*pressed_keys, KEY_W))
        {
            if (isModifier(*pressed_keys, MODIFIER_OnlyShift))
            {
                // Shift - вставляем реверсивку
                insertReversHandle(true);

                revers_shaft_timer.stop();
            }
            else
            {
                if (isModifier(*pressed_keys, MODIFIER_OnlyControl))
                {
                    // Ctrl - извлекаем реверсивку
                    insertReversHandle(false);

                    revers_shaft_timer.stop();
                }
                else
                {
                    rs_dir = 1;

                    revers_shaft_timer.start();
                }
            }
        }

        if (getKeyState(*pressed_keys, KEY_S))
        {
            rs_dir = -1;

            revers_shaft_timer.start();
        }
    }
    else
    {
        revers_shaft_timer.stop();
    }


    main_shaft_timer.step(t, dt);
    revers_shaft_timer.step(t, dt);
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void ControllerKM2202::slotRotateMainShaft()
{
    setPosition(ms_position + ms_dir);
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void ControllerKM2202::slotRotateReversShaft()
{
    setReversHandlePos(rs_position + rs_dir);
}
