#include    "tep70bs.h"

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void TEP70BS::stepOilSystem(const double& t, const double& dt)
{
    electro_oil_pump->setVoltage(Ucc * static_cast<double>(kontaktor_oil_pump->getContactState(0)));
    electro_oil_pump->step(t, dt);
}
