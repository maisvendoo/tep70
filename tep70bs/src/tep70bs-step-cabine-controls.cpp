#include    "tep70bs.h"

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void TEP70BS::stepCabineControls(const double& t, const double& dt)
{
    km[CAB1]->step(t, dt);
    km[CAB2]->step(t, dt);
}
