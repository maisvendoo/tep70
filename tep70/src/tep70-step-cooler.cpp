#include    "tep70.h"

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void TEP70::stepCooler(const double& t, const double& dt)
{
    // Уставки температуры воды контуров
    double T_min = 70.0;
    double T_max = 92.0;

    // Пределы регулирования шайбы гидронасоса привода вентиляторов
    double gamma_min = 0.05;
    double gamma_max = 1.0;

    // Гидропривод вентилятора горячего контура
    rtv_fan->setValue(disel->getWaterTemp());

    double gamma_hot = gamma_min;

    if (rtv_fan->getState())
    {
        gamma_hot = cut(gamma_min + (gamma_max - gamma_min) *
                        (disel->getWaterTemp() - T_min) / (T_max - T_min),
                        gamma_min, gamma_max);
    }

    fan_hot->setSwashPlate(gamma_hot);
    fan_hot->setDiselOmega(disel->getOmega());
    fan_hot->setFanOmega(fan_hot_aero->getFanOmega());
    fan_hot->step(t, dt);

    // Вентилятор горячего контура
    fan_hot_aero->setDriveTorque(fan_hot->getDriveTorque());
    fan_hot_aero->step(t, dt);

    // Гидропривод вентилятора холодного контура
    rtm_fan->setValue(disel->getWaterColdTemp());

    double gamma_cold = gamma_min;

    if (rtm_fan->getState())
    {
        gamma_cold = cut(gamma_min + (gamma_max - gamma_min) *
                         (disel->getWaterColdTemp() - T_min) / (T_max - T_min),
                         gamma_min, gamma_max);
    }

    fan_cold->setSwashPlate(gamma_cold);
    fan_cold->setDiselOmega(disel->getOmega());
    fan_cold->setFanOmega(fan_cold_aero->getFanOmega());
    fan_cold->step(t, dt);

    // Вентилятор холодного контура
    fan_cold_aero->setDriveTorque(fan_cold->getDriveTorque());
    fan_cold_aero->step(t, dt);

    // Передаём обороты вентиляторов в дизель для расчета охлаждения
    disel->setFanHotOmega(fan_hot_aero->getFanOmega());
    disel->setFanColdOmega(fan_cold_aero->getFanOmega());

    // Термореле жалюзи
    rtv->setValue(disel->getWaterTemp());
    rtm->setValue(disel->getOilTemp());

    // Управление ВЖВ (жалюзи воды)
    int active_cab = cabine_switcher->isCabine2() ? CAB2 : CAB1;
    int water_pos = tumbler_water_zaluzi[active_cab].getPosition();

    bool vzhv_auto = rtv->getState();
    bool vzhv_on = false;

    if (water_pos == 0)
        vzhv_on = true;
    else if (water_pos == 1)
        vzhv_on = false;
    else if (water_pos == 2)
        vzhv_on = vzhv_auto;

    vzhv->setVoltage(Ucc * static_cast<double>(vzhv_on));
    vzhv->step(t, dt);

    // Управление ВЖМ (жалюзи масла)
    int oil_pos = tumbler_oil_zaluzi[active_cab].getPosition();

    bool vzhm_auto = rtm->getState();
    bool vzhm_on = false;

    if (oil_pos == 0)
        vzhm_on = true;
    else if (oil_pos == 1)
        vzhm_on = false;
    else if (oil_pos == 2)
        vzhm_on = vzhm_auto;

    vzhm->setVoltage(Ucc * static_cast<double>(vzhm_on));
    vzhm->step(t, dt);

    // Передаём состояние жалюзи в дизель
    water_blinds_open = vzhv_on && vzhv->isOpened();
    oil_blinds_open = vzhm_on && vzhm->isOpened();
    disel->setWaterBlindsState(water_blinds_open);
    disel->setOilBlindsState(oil_blinds_open);
}
