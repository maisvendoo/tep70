#include    "tep70.h"
#include    "tep70-controls.h"

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void TEP70::stepElectroTransmission(const double& t, const double& dt)
{
    // Ток, потребляемый от главного генератора
    I_gen = 0.0;

    // Микропереключатели крана машиниста для контроля ЭТ
    bool is_KMT_ON1 = brake_crane[CAB1]->getPositionName() != "VI";
    bool is_KMT_ON2 = brake_crane[CAB2]->getPositionName() != "VI";

    // Цепь реле РУ9 - контроль сбора тяги от ЭПК (В будущем добавить другие блокировки)
    bool is_RU9_ON1 = KB1_switch->getContactState(KB1_CONTACT_CAB1_K8) && is_KMT_ON1 && (!epk[CAB1]->getEmergencyBrakeContact());
    bool is_RU9_ON2 = KB1_switch->getContactState(KB1_CONTACT_CAB2_K16) && is_KMT_ON2 && (!epk[CAB2]->getEmergencyBrakeContact());

    ru9->setVoltage(Ucc * static_cast<double>(is_RU9_ON1 || is_RU9_ON2));
    ru9->step(t, dt);

    // Состояние провода 819
    bool is_819_ON1 = KB1_switch->getContactState(KB1_CONTACT_CAB1_K6) &&
                      (km[CAB1]->isNoZero() || button_maneuver[CAB1].getState()) &&
                      brake_lock[CAB1]->isStateOn() &&
                      epk[CAB1]->isKeyOn() && azv_upr_tepl[CAB1].getState();

    bool is_819_ON2 = KB1_switch->getContactState(KB1_CONTACT_CAB2_K14) &&
                      (km[CAB2]->isNoZero() || button_maneuver[CAB2].getState()) &&
                      brake_lock[CAB2]->isStateOn() &&
                      epk[CAB2]->isKeyOn() && azv_upr_tepl[CAB2].getState();

    // Цепь катушки реле РУ3 - контроль температуры дизеля
    bool is_Water_overheat = disel->getWaterTemp() >= disel->getWaterTempAlarm();

    bool is_Oil_overheat = disel->getOilTemp() >= disel->getOilTempAlarm();

    bool is_RU3_ON = (is_Revers_Forward || is_Revers_Backward) &&
                     (is_Water_overheat || is_Oil_overheat);

    // Цепь блокировок тяги
    bool is_Traction_allowed = ru9->getContactState(RU9_EPK_CTRL) && // от ЭПК
                               ru3->getContactState(RU3_TRACTION_ALLOW) && // по температуре
                               is_DP_closed; // по дверям ВВК

    ru3->setVoltage(Ucc * static_cast<double>(is_RU3_ON));
    ru3->step(t, dt);

    // Контроль дверей ВВК
    is_DP_closed = (!control_inputs[CABS_NUM][CTRL_VVK_DOOR].toBool()) &&
                   (!control_inputs[CABS_NUM][CTRL_VVK_BLINDS_DOOR].toBool());

    is_819_ON = is_819_ON1 || is_819_ON2;    

    // Состояние цепи поездных контакторов
    is_KP_on = is_819_ON && brake_switcher->isTraction() && is_Traction_allowed;

    tracForce = 0;

    // Состояние последовательной цепи размыкающих контактов КП1 - КП7
    bool is_KP1_KP7_off = true;

    // Состояние последовательной цепи замыкающих контактор КП1 - КП6
    bool is_KP1_KP6_on = true;

    for (size_t i = 0; i < motor.size(); ++i)
    {
        kp[i]->setVoltage(Ucc * static_cast<double>(is_KP_on) * static_cast<double>(!tumbler_om[i].getState()));
        kp[i]->step(t, dt);

        motor[i]->setAncorVoltage(trac_gen->getVoltage() * static_cast<double>(kp[i]->getContactState(0)));
        motor[i]->setOmega(ip * wheel_omega[i]);

        // Ослабление возбуждения
        double beta = 1.0 - 0.42 * static_cast<double>(ksh1->getContactState(0))
                          - 0.24 * static_cast<double>(ksh2->getContactState(0));

        motor[i]->setFieldWeak(beta);

        motor[i]->setReversSate(reversor->getSatate());

        motor[i]->setMode(brake_switcher->getState() * static_cast<int>(kp[i]->getContactState(0)));

        motor[i]->step(t, dt);

        Q_a[i+1] = motor[i]->getTorque() * ip;

        tracForce += Q_a[i+1] * 2 / wheel_diameter[i];

        I_gen += motor[i]->getAncorCurrent() * static_cast<double>(kp[i]->getContactState(0));

        is_KP1_KP7_off = is_KP1_KP7_off && kp[i]->getContactState(1);
        is_KP1_KP6_on = is_KP1_KP6_on && (kp[i]->getContactState(2) || tumbler_om[i].getState());
    }

    is_KP1_KP7_off = is_KP1_KP7_off && kp[6]->getContactState(1);

    // Цепь РУ4 - сбор тяги на 1-й позиции контроллера.
    // Катушка реле получает питание через блокировки от КМ на любой его позиции,
    // кроме первой. Своими нормально замкнутым контактов оно шунтирует контакт
    // самопитания КВВ, позволяя собрать схему тяги ТОЛЬКО на первой позиии
    bool is_RU4_ON1 = KB1_switch->getContactState(KB1_CONTACT_CAB1_K6) &&
                      azv_upr_tepl[CAB1].getState() &&
                      km[CAB1]->isMoreFirst();

    bool is_RU4_ON2 = KB1_switch->getContactState(KB1_CONTACT_CAB2_K14) &&
                      azv_upr_tepl[CAB2].getState() &&
                      km[CAB2]->isMoreFirst();

    bool is_RU4_ON = is_RU4_ON1 || is_RU4_ON2;

    ru4->setVoltage(Ucc * static_cast<double>(is_RU4_ON));
    ru4->step(t, dt);

    // Состояние цепи контактора возбуждения генератора
    is_KVG_on = is_Traction_allowed && is_KP1_KP6_on && (kvv->getContactState(KVV_SELF_POWER) || ru4->getContactState(RU4_KVV_POWER));

    kvg->setVoltage(Ucc * static_cast<double>(is_KVG_on));
    kvg->step(t, dt);

    // Состояние цепи контактора возбуждения возбудителя. Катушка питается от той же
    // цепи что и КВГ, через диод
    is_KVV_on = is_KVG_on;

    kvv->setVoltage(Ucc * static_cast<double>(is_KVV_on));
    kvv->step(t, dt);    

    // Состояние цепи обмотки возбуждения возбудителя
    bool is_FGF_on = kontaktor_starter->getContactState(1) ||
                     kvv->getContactState(KVV_CONTACT_MAIN);

    field_gen->setFieldVoltage(Ucc * static_cast<double>(is_FGF_on));
    field_gen->setLoadCurrent(trac_gen->getFieldCurrent());
    field_gen->setOmega(disel->getStarterOmega());
    field_gen->step(t, dt);

    // Состояние цепи возбуждения главного генератора
    bool is_TGF_on = kvg->getContactState(0);

    trac_gen->setFieldVoltage(field_reg->getFieldVoltage() * static_cast<double>(is_TGF_on));
    trac_gen->setLoadCurrent(I_gen);
    trac_gen->setOmega(disel->getOmega());
    trac_gen->step(t, dt);

    field_reg->setActive(kvg->getContactState(2));
    field_reg->setFGVoltage(field_gen->getVoltage());
    field_reg->setOmega(disel->getOmega());
    field_reg->setGenVoltage(trac_gen->getVoltage());
    field_reg->setGenCurrent(I_gen);

    // Имитируем переключение питания с КМ по кабинам, в цепях, связанных с регулировкой мощности

    int km_pos = 0;

    if (button_maneuver[CAB1].getState() || button_maneuver[CAB2].getState())
    {
        // При нажатой кнопке "Маневр" мощность не выше 1-й позиции
        km_pos = static_cast<int>(km[CAB1]->isZero() || km[CAB2]->isZero());
    }
    else
    {
        km_pos = static_cast<double>(KB1_switch->getContactState(KB1_CONTACT_CAB1_K6) * km[CAB1]->getPositionNumber()) +
                 static_cast<double>(KB1_switch->getContactState(KB1_CONTACT_CAB2_K14) * km[CAB2]->getPositionNumber());
    }

    field_reg->setKMPosition(km_pos);
    field_reg->step(t, dt);

    // Цепь реле РУ1
    bool is_RU1_on = is_819_ON &&
                     km[CAB1]->is12orMore() && KB1_switch->getContactState(KB1_CONTACT_CAB1_K6) &&
                     km[CAB2]->is12orMore() && KB1_switch->getContactState(KB1_CONTACT_CAB2_K14);

    ru1->setVoltage(Ucc * static_cast<double>(is_RU1_on));
    ru1->step(t, dt);

    double k_field = 0;

    // Работа реле перехода
    if (I_gen >= 1.0)
        k_field = trac_gen->getVoltage() / I_gen;

    bool is_OP1_manual = ((tumbler_field_weak1[CAB1].getPosition() == 0) && KB1_switch->getContactState(KB1_CONTACT_CAB1_K6)) ||
                         ((tumbler_field_weak1[CAB2].getPosition() == 0) && KB1_switch->getContactState(KB1_CONTACT_CAB2_K14));

    bool is_OP1_auto =   ((tumbler_field_weak1[CAB1].getPosition() == 2) && KB1_switch->getContactState(KB1_CONTACT_CAB1_K6)) ||
                         ((tumbler_field_weak1[CAB2].getPosition() == 2) && KB1_switch->getContactState(KB1_CONTACT_CAB2_K14));

    bool is_OP2_manual = ((tumbler_field_weak2[CAB1].getPosition() == 0) && KB1_switch->getContactState(KB1_CONTACT_CAB1_K6)) ||
                         ((tumbler_field_weak2[CAB2].getPosition() == 0) && KB1_switch->getContactState(KB1_CONTACT_CAB2_K14));

    bool is_OP2_auto =   ((tumbler_field_weak2[CAB1].getPosition() == 2) && KB1_switch->getContactState(KB1_CONTACT_CAB1_K6)) ||
                         ((tumbler_field_weak2[CAB2].getPosition() == 2) && KB1_switch->getContactState(KB1_CONTACT_CAB2_K14));

    rp1->setActive(is_OP1_auto);
    rp1->setLocked(ksh1_delay->getContactState(0));
    rp1->setValue(k_field);

    rp2->setActive( (is_OP2_auto) && ksh2_delay->getContactState(0) );
    rp2->setValue(k_field);

    // Цепь контактора КШ2
    bool is_KSH2_on = ( (is_OP2_manual) &&
                      ru1->getContactState(1) ) ||
                      ( (is_OP2_auto) && rp2->getState() );

    // Цепь контактора КШ1
    bool is_KSH1_on = ( (is_OP1_manual) &&
                      ru1->getContactState(0) ) ||
                      ( rp1->getState()  && is_OP1_auto);

    ksh2_delay->setControlVoltage(Ucc * static_cast<double>(ksh1->getContactState(1)));
    ksh2_delay->step(t, dt);

    ksh1_delay->setControlVoltage(Ucc * static_cast<double>(ksh2->getContactState(2)));
    ksh1_delay->step(t, dt);

    ksh1->setVoltage(Ucc * static_cast<double>(is_KSH1_on));
    ksh1->step(t, dt);

    ksh2->setVoltage(Ucc * static_cast<double>(is_KSH2_on));
    ksh2->step(t, dt);


    // Цепь вентиля реверсора вперед
    is_Revers_Forward = is_819_ON && (km[CAB1]->isForward() || km[CAB2]->isBackward());

    // Цепь вентиля реверсора назад
    is_Revers_Backward = is_819_ON && (km[CAB1]->isBackward() || km[CAB2]->isForward());

    reversor->setForwardValveState(is_Revers_Forward);
    reversor->setBackwardValveState(is_Revers_Backward);
    reversor->step(t, dt);

    // Цепь реле РУ21
    ru21->setVoltage(0);
    ru21->step(t, dt);

    // Цепь тормозного контактора
    kt->setVoltage(0);
    kt->step(t, dt);

    // Цепь вентиля "Тяга" тормозного переключателя
    bool is_TRAC_on = is_819_ON &&
                      (is_KP1_KP7_off || (brake_switcher->isTraction() && ru21->getContactState(RU21_TRAC_ON)));


    brake_switcher->setTracValeState(is_TRAC_on);
    brake_switcher->setBrakeValveState(false);
    brake_switcher->step(t, dt);
}
