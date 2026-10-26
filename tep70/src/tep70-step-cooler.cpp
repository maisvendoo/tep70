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

    // Цепи управления жалюзи горячего контура
    bool is_vzhv_CAB1_ON = (tumbler_water_zaluzi[CAB1].getPosition() == 0) ||
                           ((tumbler_water_zaluzi[CAB1].getPosition() == 2) && rtv->getState());

    bool is_vzhv_CAB2_ON = (tumbler_water_zaluzi[CAB2].getPosition() == 0) ||
                           ((tumbler_water_zaluzi[CAB2].getPosition() == 2) && rtv->getState());

    bool is_vzhv_ON = (is_vzhv_CAB1_ON && KB1_switch->getContactState(KB1_CONTACT_CAB1_K6)) ||
                      (is_vzhv_CAB2_ON && KB1_switch->getContactState(KB1_CONTACT_CAB2_K14));

    vzhv->setVoltage(Ucc * static_cast<double>(is_vzhv_ON));
    vzhv->step(t, dt);

    // Управление холодного контура
    bool is_vzhm_CAB1_ON = (tumbler_oil_zaluzi[CAB1].getPosition() == 0) ||
                           ((tumbler_oil_zaluzi[CAB1].getPosition() == 2) && rtm->getState());

    bool is_vzhm_CAB2_ON = (tumbler_oil_zaluzi[CAB2].getPosition() == 0) ||
                           ((tumbler_oil_zaluzi[CAB2].getPosition() == 2) && rtm->getState());

    bool is_vzhm_ON = (is_vzhm_CAB1_ON && KB1_switch->getContactState(KB1_CONTACT_CAB1_K6)) ||
                      (is_vzhm_CAB2_ON && KB1_switch->getContactState(KB1_CONTACT_CAB2_K14));

    vzhm->setVoltage(Ucc * static_cast<double>(is_vzhm_ON));
    vzhm->step(t, dt);

    // Передаём состояние жалюзи в дизель
    water_blinds_open = is_vzhv_ON && vzhv->isOpened();
    oil_blinds_open = is_vzhm_ON && vzhm->isOpened();
    disel->setWaterBlindsState(water_blinds_open);
    disel->setOilBlindsState(oil_blinds_open);
}
