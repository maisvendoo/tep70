#include    "tep70.h"

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void TEP70::stepOther(const double& t, const double& dt)
{
    horn->setFLpressure(main_reservoir->getPressure());
    horn->step(t, dt);

    bool is_svistok = button_svistok[CAB1].getState() ||
                      button_svistok[CAB2].getState() ||
                      button_svistok_pom[CAB1].getState() ||
                      button_svistok_pom[CAB2].getState() ||
                      button_svistok_maneuver[CAB1].getState() ||
                      button_svistok_maneuver[CAB2].getState();

    horn->setSvistokOn(is_svistok && azv_control_circuit.getState());

    bool is_tifon = button_tifon[CAB1].getState() ||
                    button_tifon[CAB2].getState() ||
                    button_tifon_pom[CAB1].getState() ||
                    button_tifon_pom[CAB2].getState();

    horn->setTifonOn(is_tifon && azv_control_circuit.getState());

    // Система подачи песка
    sand_system->setFLpressure(main_reservoir->getPressure());
    sand_system->step(t, dt);

    // Состояние пескоподачи из кнопки (управляется от IOController в stepControls).
    // Ставится после step(), т.к. stepKeysControl иначе сбросит его в false.
    sand_system->setSandDeliveryOn(button_sand[CAB1].getState() ||
                                   button_sand[CAB2].getState());

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
