#include    "tep70.h"

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void TEP70::stepOther(const double& t, const double& dt)
{
    horn->setFLpressure(main_reservoir->getPressure());
    horn->setSvistokOn(button_svistok[CAB1].getState() || button_svistok[CAB2].getState());
    horn->setTifonOn(button_tifon[CAB1].getState() || button_tifon[CAB2].getState());
    horn->step(t, dt);

    // Система подачи песка
    sand_system->setFLpressure(main_reservoir->getPressure());
    sand_system->step(t, dt);
    for (size_t i = 0; i < num_axis; ++i)
    {
        // Пересчёт трения колесо-рельс
        psi[i] = sand_system->getWheelRailFrictionCoeff(psi[i]);
    }
    // Пересчёт массы локомотива
    payload_coeff = sand_system->getSandLevel();
    // Пересчёт не нужен, так как выполняется в stepFuelSystem()
    //setPayloadCoeff(payload_coeff);
}
