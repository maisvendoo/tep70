#include    "tep70.h"

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void TEP70::stepCabineControls(const double& t, const double& dt)
{
    km[CAB1]->step(t, dt);
    km[CAB2]->step(t, dt);
}
