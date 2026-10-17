#include    "tep70.h"

#include    <tep70-controls.h>

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void TEP70::stepOther(const double& t, const double& dt)
{
    horn->setFLpressure(main_reservoir->getPressure());
    horn->step(t, dt);
    horn->setSvistokOn(button_svistok[CAB1].getState() || button_svistok[CAB2].getState() ||
                       button_svistok_pom[CAB1].getState() || button_svistok_pom[CAB2].getState());
    horn->setTifonOn(button_tifon[CAB1].getState() || button_tifon[CAB2].getState() ||
                     button_tifon_pom[CAB1].getState() || button_tifon_pom[CAB2].getState());

    // Система подачи песка
    sand_system->setFLpressure(main_reservoir->getPressure());
    sand_system->step(t, dt);

    // Состояние пескоподачи от сигналов IOController (кнопка на пульте каждой кабины).
    // Ставится после step(), т.к. stepKeysControl иначе сбросит его в false.
    sand_system->setSandDeliveryOn(control_inputs[CAB1][CTRL_SAND_BUTTON].toBool() ||
                                   control_inputs[CAB2][CTRL_SAND_BUTTON].toBool());

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
