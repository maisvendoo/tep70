#include    "tep70.h"

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void TEP70::keyProcess(const simulator_time_t& t, const double& dt)
{
    // Песочница
    sand_system->setControl(&pressed_keys);
    // Тифон, свисток
    horn->setControl(&pressed_keys);

    // Автозапуск (legacy через pressed_keys)
    if (autoStartTimer->isStarted())
    {
        return;
    }

    if (getKeyState(KEY_R, CAB1) && isAlt(CAB1) && initAutostartProgram(CAB1))
    {
        autoStartTimer->start();
        return;
    }

    if (getKeyState(KEY_R, CAB2) && isAlt(CAB2) && initAutostartProgram(CAB2))
    {
        autoStartTimer->start();
        return;
    }

    // Автоостанов (legacy через pressed_keys)
    if (getKeyState(KEY_T, CAB1) && isAlt(CAB1) && initAutostopProgram(CAB1))
    {
        autoStartTimer->start();
        return;
    }

    if (getKeyState(KEY_T, CAB2) && isAlt(CAB2) && initAutostopProgram(CAB2))
    {
        autoStartTimer->start();
        return;
    }

    // Сигналы от внешнего пульта FreeJoy
    for (auto cab_idx : {CAB1, CAB2})
    {
        if (control_signals.analogSignal[CS_BRAKE_CRANE].is_active)
        {
            int brake_crane_pos = static_cast<int>(control_signals.analogSignal[CS_BRAKE_CRANE].cur_value);
            brake_crane[cab_idx]->setHandlePosition(brake_crane_pos);
        }

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

        if (control_signals.analogSignal[CS_RBS].is_active)
        {
            if (static_cast<bool>(control_signals.analogSignal[CS_RBS].cur_value))
                rb[cab_idx][RBS].set();
            else
                rb[cab_idx][RBS].reset();
        }
    }
}