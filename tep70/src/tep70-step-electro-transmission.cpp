#include    "tep70.h"

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void TEP70::stepElectroTransmission(double t, double dt)
{
    // Ток, потребляемый от главного генератора
    I_gen = 0.0;

    // Микропереключатели крана машиниста для контроля ЭТ
    bool is_KMT_ON1 = brake_crane[CAB1]->getPositionName() != "VI";
    bool is_KMT_ON2 = brake_crane[CAB2]->getPositionName() != "VI";

    // Цепь реле РУ9 - контроль сбора тяги от ЭПК (В будущем добавить другие блокировки)    
    bool is_RU9_ON1 = cabine_switcher->isCabine1() && is_KMT_ON1 && (!epk[CAB1]->getEmergencyBrakeContact());
    bool is_RU9_ON2 = cabine_switcher->isCabine2() && is_KMT_ON2 && (!epk[CAB2]->getEmergencyBrakeContact());

    ru9->setVoltage(Ucc * static_cast<double>(is_RU9_ON1 || is_RU9_ON2));
    ru9->step(t, dt);

    // Состояние провода 819
    bool is_819_ON1 = cabine_switcher->isCabine1() &&
                      km[CAB1]->isNoZero() &&
                      brake_lock[CAB1]->isUnlocked() &&
                      key_epk[CAB1].getState() && azv_upr_tepl[CAB1].getState();

    bool is_819_ON2 = cabine_switcher->isCabine2() &&
                      km[CAB2]->isNoZero() &&
                      brake_lock[CAB2]->isUnlocked() &&
                      key_epk[CAB2].getState() && azv_upr_tepl[CAB2].getState();

    bool is_819_ON = ru9->getContactState(RU9_EPK_CTRL) &&
                     (is_819_ON1 || is_819_ON2);

    // Состояние цепи поездных контакторов
    bool is_KP_on = azv_upr_tepl[CAB1].getState() && brake_switcher->isTraction();

    tracForce = 0;

    // Состояние последовательной цепи размыкающих контактов КП1 - КП7
    bool is_KP1_KP7_off = true;

    // Состояние последовательной цепи замыкающих контактор КП1 - КП6
    bool is_KP1_KP6_on = true;

    for (size_t i = 0; i < motor.size(); ++i)
    {
        kp[i]->setVoltage(Ucc * static_cast<double>(is_KP_on));
        kp[i]->step(t, dt);

        motor[i]->setAncorVoltage(trac_gen->getVoltage() * static_cast<double>(kp[i]->getContactState(0)));
        motor[i]->setOmega(ip * wheel_omega[i]);

        // Ослабление возбуждения
        double beta = 1.0 - 0.42 * static_cast<double>(ksh1->getContactState(0))
                          - 0.24 * static_cast<double>(ksh2->getContactState(0));

        motor[i]->setFieldWeak(beta);

        motor[i]->setReversSate(reversor->getSatate());

        motor[i]->setMode(brake_switcher->getState());

        motor[i]->step(t, dt);

        Q_a[i+1] = motor[i]->getTorque() * ip;

        tracForce += Q_a[i+1] * 2 / wheel_diameter[i];

        I_gen += motor[i]->getAncorCurrent();

        is_KP1_KP7_off = is_KP1_KP7_off && kp[i]->getContactState(1);
        is_KP1_KP6_on = is_KP1_KP6_on && kp[i]->getContactState(2);
    }

    is_KP1_KP7_off = is_KP1_KP7_off && kp[6]->getContactState(1);

    // Состояние цепи контактора возбуждения возбудителя
    bool is_KVV_on = azv_upr_tepl[CAB1].getState() && is_KP1_KP6_on;

    kvv->setVoltage(Ucc * static_cast<double>(is_KVV_on));
    kvv->step(t, dt);

    // Состояние цепи контактора возбуждения генератора
    bool is_KVG_on = azv_upr_tepl[CAB1].getState() && is_KP1_KP6_on;

    kvg->setVoltage(Ucc * static_cast<double>(is_KVG_on));
    kvg->step(t, dt);

    // Состояние цепи обмотки возбуждения возбудителя
    bool is_FGF_on = kontaktor_starter->getContactState(1) ||
                     kvv->getContactState(0);

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
    field_reg->setKMPosition(km[CAB1]->getPositionNumber());
    field_reg->step(t, dt);

    // Цепь реле РУ1
    bool is_RU1_on = azv_upr_tepl[CAB1].getState() && km[CAB1]->is12orMore();

    ru1->setVoltage(Ucc * static_cast<double>(is_RU1_on));
    ru1->step(t, dt);

    double k_field = 0;

    // Работа реле перехода
    if (I_gen >= 1.0)
        k_field = trac_gen->getVoltage() / I_gen;

    rp1->setActive(tumbler_field_weak1.getPosition() == 2);
    rp1->setLocked(ksh1_delay->getContactState(0));
    rp1->setValue(k_field);

    rp2->setActive( (tumbler_field_weak2.getPosition() == 2) && ksh2_delay->getContactState(0) );
    rp2->setValue(k_field);

    // Цепь контактора КШ2
    bool is_KSH2_on = ( (tumbler_field_weak2.getPosition() == 0) &&
                      ru1->getContactState(1) ) ||
                      ( (tumbler_field_weak2.getPosition() == 2) && rp2->getState() );

    // Цепь контактора КШ1
    bool is_KSH1_on = ( (tumbler_field_weak1.getPosition() == 0) &&
                      ru1->getContactState(0) ) ||
                      ( rp1->getState()  && tumbler_field_weak1.getPosition() == 2);

    ksh2_delay->setControlVoltage(Ucc * static_cast<double>(ksh1->getContactState(1)));
    ksh2_delay->step(t, dt);

    ksh1_delay->setControlVoltage(Ucc * static_cast<double>(ksh2->getContactState(2)));
    ksh1_delay->step(t, dt);

    ksh1->setVoltage(Ucc * static_cast<double>(is_KSH1_on));
    ksh1->step(t, dt);

    ksh2->setVoltage(Ucc * static_cast<double>(is_KSH2_on));
    ksh2->step(t, dt);


    // Цепь вентиля реверсора вперед
    bool is_Revers_Forward = azv_upr_tepl[CAB1].getState() && km[CAB1]->isForward();

    // Цепь вентиля реверсора назад
    bool is_Revers_Backward = azv_upr_tepl[CAB1].getState() && km[CAB1]->isBackward();

    reversor->setForwardValveState(is_Revers_Forward);
    reversor->setBackwardValveState(is_Revers_Backward);
    reversor->step(t, dt);

    // Цепь реле РУ21
    ru21->setVoltage(0);
    ru21->step(t, dt);

    // Цепь вентиля "Тяга" тормозного переключателя
    bool is_TRAC_on = is_819_ON &&
                      (is_KP1_KP7_off || (brake_switcher->isTraction() && ru21->getContactState(RU21_TRAC_ON)));


    brake_switcher->setTracValeState(is_TRAC_on);
    brake_switcher->setBrakeValveState(false);
    brake_switcher->step(t, dt);
}
