#include    "tep70.h"

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void TEP70::stepCooler(const double& t, const double& dt)
{
    // 1. Гидропривод горячего контура
    fan_hot->setWaterTemp(disel->getWaterTemp());
    fan_hot->setDiselOmega(disel->getOmega());
    fan_hot->setFanOmega(fan_hot_aero->getFanOmega());
    fan_hot->step(t, dt);

    // 2. Вентилятор горячего контура
    fan_hot_aero->setDriveTorque(fan_hot->getDriveTorque());
    fan_hot_aero->step(t, dt);

    // 3. Гидропривод холодного контура
    fan_cold->setWaterTemp(disel->getWaterColdTemp());
    fan_cold->setDiselOmega(disel->getOmega());
    fan_cold->setFanOmega(fan_cold_aero->getFanOmega());
    fan_cold->step(t, dt);

    // 4. Вентилятор холодного контура
    fan_cold_aero->setDriveTorque(fan_cold->getDriveTorque());
    fan_cold_aero->step(t, dt);

    // 5. Передаём обороты вентиляторов в дизель
    disel->setFanHotOmega(fan_hot_aero->getFanOmega());
    disel->setFanColdOmega(fan_cold_aero->getFanOmega());

    // 6. Термореле
    rtv->setValue(disel->getWaterTemp());
    rtm->setValue(disel->getOilTemp());

    // 7. Управление ВЖВ (жалюзи воды)
    int active_cab = cabine_switcher->isCabine2() ? CAB2 : CAB1;
    int water_pos = tumbler_water_zaluzi[active_cab].getPosition();

    bool vzhv_auto = rtv->getState();
    bool vzhv_on = false;

    if (water_pos == 0)               // Ручное / Закрыть
        vzhv_on = true;
    else if (water_pos == 1)          // Выключено (цепь разорвана)
        vzhv_on = false;
    else if (water_pos == 2)          // Авто
        vzhv_on = vzhv_auto;

    vzhv->setVoltage(Ucc * static_cast<double>(vzhv_on));
    vzhv->step(t, dt);

    // 8. Управление ВЖМ (жалюзи масла)
    int oil_pos = tumbler_oil_zaluzi[active_cab].getPosition();

    bool vzhm_auto = rtm->getState();
    bool vzhm_on = false;

    if (oil_pos == 0)                 // Ручное / Открыть
        vzhm_on = true;
    else if (oil_pos == 1)            // Выключено (цепь разорвана)
        vzhm_on = false;
    else if (oil_pos == 2)            // Авто
        vzhm_on = vzhm_auto;

    vzhm->setVoltage(Ucc * static_cast<double>(vzhm_on));
    vzhm->step(t, dt);

    // 9. Передаём состояние жалюзи в дизель
    water_blinds_open = vzhv_on && vzhv->isOpened();
    oil_blinds_open = vzhm_on && vzhm->isOpened();
    disel->setWaterBlindsState(water_blinds_open);
    disel->setOilBlindsState(oil_blinds_open);
}