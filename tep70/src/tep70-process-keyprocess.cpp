#include    "tep70.h"

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void TEP70::keyProcess(const simulator_time_t& t, const double& dt)
{
    // Песочница — состояние задаётся от IOController в stepOther(), клавиши не используются
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
}