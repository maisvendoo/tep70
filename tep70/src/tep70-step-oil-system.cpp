#include    "tep70.h"

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void TEP70::stepOilSystem(const double& t, const double& dt)
{
    electro_oil_pump->setVoltage(Ucc * static_cast<double>(kontaktor_oil_pump->getContactState(0)));
    electro_oil_pump->step(t, dt);
}
