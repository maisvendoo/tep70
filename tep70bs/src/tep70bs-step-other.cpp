#include    "tep70bs.h"

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void TEP70BS::stepOther(double t, double dt)
{
    horn->setFLpressure(main_reservoir->getPressure());
    horn->setControl(keys);
    horn->step(t, dt);
}
