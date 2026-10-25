#include    "tep70.h"

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void TEP70::stepCooler(const double& t, const double& dt)
{
    // 1. Вентиляторы (всегда работают в автомате)
    fan_hot->setWaterTemp(disel->getWaterTemp());
    fan_hot->setDiselOmega(disel->getOmega());
    fan_hot->step(t, dt);

    fan_cold->setWaterTemp(disel->getWaterColdTemp());
    fan_cold->setDiselOmega(disel->getOmega());
    fan_cold->step(t, dt);

    // 2. Передаём обороты вентиляторов в дизель
    disel->setFanHotOmega(fan_hot->getFanOmega());
    disel->setFanColdOmega(fan_cold->getFanOmega());

    // 3. Термореле
    rtv->setValue(disel->getWaterTemp());
    rtm->setValue(disel->getOilTemp());

    // 4. Управление ВЖВ (жалюзи воды)
    int active_cab = cabine_switcher->isCabine2() ? CAB2 : CAB1;
    int water_pos = tumbler_water_zaluzi[active_cab].getPosition();

    bool vzhv_auto = rtv->getState();
    bool vzhv_on = false;

    if (water_pos == 0)               // "Закрыть"
        vzhv_on = false;
    else if (water_pos == 1)          // "Авто"
        vzhv_on = vzhv_auto;
    else if (water_pos == 2)          // "Открыть"
        vzhv_on = true;

    vzhv->setVoltage(Ucc * static_cast<double>(vzhv_on));
    vzhv->step(t, dt);

    // 5. Управление ВЖМ (жалюзи масла)
    int oil_pos = tumbler_oil_zaluzi[active_cab].getPosition();

    bool vzhm_auto = rtm->getState();
    bool vzhm_on = false;

    if (oil_pos == 0)                 // "Закрыть"
        vzhm_on = false;
    else if (oil_pos == 1)            // "Авто"
        vzhm_on = vzhm_auto;
    else if (oil_pos == 2)            // "Открыть"
        vzhm_on = true;

    vzhm->setVoltage(Ucc * static_cast<double>(vzhm_on));
    vzhm->step(t, dt);

    // 6. Передаём состояние жалюзи в дизель
    disel->setWaterBlindsState(vzhv->isOpened());
    disel->setOilBlindsState(vzhm->isOpened());
}