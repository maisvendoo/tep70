#include    "tep70.h"

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void TEP70::stepOther(double t, double dt)
{
    horn->setControl(keys);
    horn->step(t, dt);

    genAmpermeter->setInputSignal(I_gen);
    genAmpermeter->step(t, dt);
}
