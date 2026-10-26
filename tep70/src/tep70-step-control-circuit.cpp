#include    "tep70.h"

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void TEP70::stepControlCircuit(const double& t, const double& dt)
{
    double U_bat = battery->getVoltage() * static_cast<float>(accum_switch.getState());

    Ucc = max(U_bat, starter_generator->getVoltage() * static_cast<double>(krn->getContactState(1)));

    power_level = static_cast<float>(Ucc / 110.0);

    // Расчитываем ток, потребляемый цепями управления
    Icc = kontaktor_fuel_pump->getCurrent() +
          electro_fuel_pump->getCurrent() +
          ru8->getCurrent() +
          kontaktor_oil_pump->getCurrent() +
          electro_oil_pump->getCurrent() +
          oilpump_time_relay->getCurrent() +
          kontaktor_starter->getCurrent() +
          starter_time_relay->getCurrent() +
          ru10->getCurrent() +
          ru6->getCurrent() +
          ru42->getCurrent() +
          mv6->getCurrent() +
          vtn->getCurrent() +
          ru4->getCurrent() +
          ru15->getCurrent() +
          rv4->getCurrent() +
          rv9->getCurrent() +
          krn->getCurrent() +
          ru18->getCurrent() +
          ktk1->getCurrent() +
          ktk2->getCurrent() +
          rv6->getCurrent();


    battery->setChargeVoltage(starter_generator->getVoltage());
    battery->setStarterCurrent(starter_generator->getAncorCurrent());

    // Правим снаружи баг в батарее, стартерный ток не влияет на напряжение
    battery->setLoadCurrent(Icc + pf(starter_generator->getAncorCurrent()));

    battery->step(t, dt);

    // Переключатель КБ1
    KB1_switch->setState(vvk_sw[VVK_KB1].getState());
    KB1_switch->step(t, dt);

    // Переключатель КБ2
    KB2_switch->setState(vvk_sw[VVK_KB2].getState());
    KB2_switch->step(t, dt);

    // Определяем состояни цепи контактора топливного насоса (КТН)

    // Питание от автомата АВ4 в кабине 1 (с учетом шунтировки от Кн4)
    bool is_AB4_CAB1_on = (KB1_switch->getContactState(KB1_CONTACT_CAB1_K2_AB4) || keep_fuel_pump_button.getState())
                          && azv_fuel_pump[CAB1].getState();

    // Питание от автомата АВ4 в кабине 2 (с учетом шунтировки от Кн4)
    bool is_AB4_CAB2_on = (KB1_switch->getContactState(KB1_CONTACT_CAB2_K10_AB4) || keep_fuel_pump_button.getState())
                          && azv_fuel_pump[CAB2].getState();


    bool is_KTH_on = (is_AB4_CAB1_on || is_AB4_CAB2_on) &&
                     tumbler_disel_stop[CAB1].getState() &&
                     tumbler_disel_stop[CAB2].getState() &&
                     ru6->getContactState(1);

    kontaktor_fuel_pump->setVoltage(Ucc * static_cast<double>(is_KTH_on));
    kontaktor_fuel_pump->step(t, dt);

    // Питание от автомата АВ2
    bool is_AB2_CAB1_on = KB1_switch->getContactState(KB1_CONTACT_CAB1_K4_AB2) && azv_common_control[CAB1].getState();
    bool is_AB2_CAB2_on = KB1_switch->getContactState(KB1_CONTACT_CAB2_K12_AB2) && azv_common_control[CAB2].getState();

    // Состояние цепи кнопки "Пуск дизеля"
    bool is_1017 = (is_AB2_CAB1_on && KB1_switch->getContactState(KB1_CONTACT_CAB1_K6) && km[CAB1]->isZero()) ||
                   (is_AB2_CAB2_on && KB1_switch->getContactState(KB1_CONTACT_CAB2_K14) && km[CAB2]->isZero());

    bool is_diser_start_ON = button_disel_start[CAB1].getState() || button_disel_start[CAB2].getState();

    bool is_Button_Start_on = is_1017 &&
                              (is_diser_start_ON || ru8->getContactState(0));

    // Определяем состояние цепи катушки реле РУ8
    bool is_RU8_on = is_Button_Start_on &&
                     ru42->getContactState(0) &&
                     kontaktor_fuel_pump->getContactState(1);

    ru8->setVoltage(Ucc * static_cast<double>(is_RU8_on));
    ru8->step(t, dt);


    // Состояние цепи контактора маслянного насоса (КМН) при пуске
    bool is_KMH_on_start = is_Button_Start_on && ru42->getContactState(0);

    // Состояние цепи КМН при остановке дизеля (прокачка после остановки)
    bool is_KMN_on_stop = is_1017 &&
                          ru6->getContactState(3) &&
                          ru15->getContactState(2);

    kontaktor_oil_pump->setVoltage(Ucc * static_cast<double>(is_KMH_on_start || is_KMN_on_stop));
    kontaktor_oil_pump->step(t, dt);

    // Состояние цепи реле времени РВ3 (задает время предпусковой прокачки)
    bool is_RV3_on = is_Button_Start_on && ru42->getContactState(0);

    oilpump_time_relay->setControlVoltage(Ucc * static_cast<double>(is_RV3_on));
    oilpump_time_relay->step(t, dt);

    // Проверяем давление масла
    bool is_RDM4_on = static_cast<bool>(hs_p(disel->getOilPressure() - 0.05));


    bool is_MV6_on = (is_AB4_CAB1_on || is_AB4_CAB2_on) &&
                     tumbler_disel_stop[CAB1].getState() &&
                     tumbler_disel_stop[CAB2].getState() &&
                    (ru8->getContactState(1) || is_RDM4_on);

    mv6->setVoltage(Ucc * static_cast<double>(is_MV6_on));
    mv6->step(t, dt);

    // Состояние цепи контактора пуска (КД)
    bool is_KD_on = ru42->getContactState(0) &&
                    oilpump_time_relay->getContactState(0) &&
                    is_RDM4_on;

    kontaktor_starter->setVoltage(Ucc * static_cast<double>(is_KD_on));
    kontaktor_starter->step(t, dt);

    starter_time_relay->setControlVoltage(Ucc * static_cast<double>(is_KD_on));
    starter_time_relay->step(t, dt);    

    // Проверяем обороты дизеля
    bool is_RU10_on = static_cast<bool>(hs_p(disel->getOmega() - 31.4)) &&
                      ru8->getContactState(2);

    ru10->setVoltage(Ucc * static_cast<double>(is_RU10_on));
    ru10->step(t, dt);

    bool is_RU6_on = ( (ru8->getContactState(3) || is_RDM4_on) &&
                     (ru10->getContactState(0) || ru6->getContactState(0)) ) ||
                     starter_time_relay->getContactState(0);

    ru6->setVoltage(Ucc * static_cast<double>(is_RU6_on));
    ru6->step(t, dt);

    bool is_VTN_on = ru6->getContactState(0) &&
                     ru4->getContactState(0) &&
                     kvg->getContactState(1);

    vtn->setVoltage(Ucc * static_cast<double>(is_VTN_on));
    vtn->step(t, dt);

    bool is_RU4_on = (KB1_switch->getContactState(KB1_CONTACT_CAB1_K6) && km[CAB1]->isMoreFirst()) ||
                     (KB1_switch->getContactState(KB1_CONTACT_CAB2_K14) && km[CAB2]->isMoreFirst());

    ru4->setVoltage(Ucc * static_cast<double>(is_RU4_on));
    ru4->step(t, dt);

    ru42->setVoltage(Ucc * static_cast<double>(is_RU6_on));
    ru42->step(t, dt);

    bool is_RU15_on = is_1017 &&
                      rv4->getContactState(0) &&
                      (ru6->getContactState(2) || ru15->getContactState(0));

    ru15->setVoltage(Ucc * static_cast<double>(is_RU15_on));
    ru15->step(t, dt);

    bool is_RV4_on = is_1017 &&
                     ru6->getContactState(3) &&
                     ru15->getContactState(1);

    rv4->setControlVoltage(Ucc * static_cast<double>(is_RV4_on));
    rv4->step(t, dt);

    rv9->setControlVoltage(Ucc * static_cast<double>(ru42->getContactState(1)));
    rv9->step(t, dt);

    krn->setVoltage(Ucc * static_cast<double>(rv9->getContactState(0)));
    krn->step(t, dt);
}
