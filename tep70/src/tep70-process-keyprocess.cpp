#include    "tep70.h"

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void TEP70::keyProcess(const simulator_time_t& t, const double& dt)
{
    // Свисток, тифон — управление через IOController в stepControls и stepOther.
    // Legacy-привязка horn->setControl удалена.
}