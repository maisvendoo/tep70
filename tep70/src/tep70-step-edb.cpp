#include    <tep70.h>

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void TEP70::stepElectroDymanicBrake(const double &t, const double &dt)
{
    EDB_blinds_actor->step(t, dt);
}
