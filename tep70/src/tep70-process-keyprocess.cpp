#include    "tep70.h"

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void TEP70::keyProcess(const simulator_time_t& t, const double& dt)
{
    // Тифон, свисток — установка legacy-привязки (будет удалено при переводе на IOController)
    horn->setControl(&pressed_keys);

    // Автозапуск и автоостанов — через IOController (CTRL_AUTOSTART_PROGRAM/CTRL_AUTOSTOP_PROGRAM)
    // в stepControls. Legacy-клавиши (Alt+R/T) удалены.
}