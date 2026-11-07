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
    bool is_RU9_ON1 = KB1_switch->getContactState(KB1_CONTACT_CAB1_K6) &&
                      ( (is_KMT_ON1 && (!epk[CAB1]->getEmergencyBrakeContact())) || shunting_tumbler.getState());

    bool is_RU9_ON2 = KB1_switch->getContactState(KB1_CONTACT_CAB2_K14) &&
                      ( (is_KMT_ON2 && (!epk[CAB2]->getEmergencyBrakeContact())) || shunting_tumbler.getState());

    ru9->setVoltage(Ucc * static_cast<double>(is_RU9_ON1 || is_RU9_ON2));
    ru9->step(t, dt);

    // Цепь провода 708: КБ1, контакт 33 КМ (поз >= 1) и параллельные контакты кнопки "Маневр",
    // АЗВ "Управление тепловозом", контакты УБТ, ключ ЭПК и параллельные контакты
    // тублера "Шунтировка ЭПКА"
    bool is_708_ON1 = KB1_switch->getContactState(KB1_CONTACT_CAB1_K6) &&
                      (km[CAB1]->isContact(KM_33) || button_maneuver[CAB1].getState()) &&
                      azv_upr_tepl[CAB1].getState() &&
                      (epk[CAB1]->isKeyOn() || shunting_tumbler.getState()) &&
                      brake_lock[CAB1]->isStateOn();

    bool is_708_ON2 = KB1_switch->getContactState(KB1_CONTACT_CAB2_K14) &&
                      (km[CAB2]->isContact(KM_33) || button_maneuver[CAB1].getState()) &&
                      azv_upr_tepl[CAB2].getState() &&
                      (epk[CAB2]->isKeyOn() || shunting_tumbler.getState()) &&
                      brake_lock[CAB2]->isStateOn();

    bool is_708_ON = is_708_ON1 || is_708_ON2;

    // Цепь провода 709: Провод 708, контакты замыкающие РУ9
    bool is_709_ON = is_708_ON && ru9->getContactState(RU9_EPK_CTRL);

    // Цепь провода 712: +AВ2 "Управление общее", замыкающий контакт КТ
    bool is_712_ON = (is_AB2_CAB1_on || is_AB2_CAB2_on) && kt->getContactState(KT_712_ON);

    // Цепь провода 710: от провода 709 или 712
    bool is_710_ON = is_709_ON || is_712_ON;

    // Цепь провода 727: провод 709 в тяге или провод 712 при ЭДТ,
    // контакты замыкающие РУ9, контакты реверсора вперед или назад.
    bool is_727_ON = is_710_ON && (is_Revers_Forward || is_Revers_Backward);

    // Цепь провода 728: провод 727, замыкающий контакт "Торможение" ТП
    is_728_ON = is_727_ON && brake_switcher->isBrake();

    // Цепь провода 737: провод 728, замыкающий контакт реле РУ21,
    // замыкающий контакт РУ32, замыкающий контакт РУ37
    bool is_737_ON = is_728_ON &&
                     ru21->getContactState(RU21_FOR_RV2_ON) &&
                     ru32->getContactState(RU32_FOR_RV2_ON) &&
                     ru37->getContactState(RU37_FOR_RV2_ON);

    // Состояние провода 819: на одной клемме с проводом 709, 712 и 710
    is_819_ON = is_710_ON;

    // Цепь катушки реле РУ3 - контроль температуры дизеля
    bool is_Water_overheat = disel->getWaterTemp() >= disel->getWaterTempAlarm();

    bool is_Oil_overheat = disel->getOilTemp() >= disel->getOilTempAlarm();

    bool is_RU3_ON = (is_Revers_Forward || is_Revers_Backward) &&
                     (is_Water_overheat || is_Oil_overheat);

    // Цепь провода 777: провод 727, контакт "Тяга" тормозного переключателя,
    // размыкающий контакт тормозного контактора КТ, размыкающий контакт РУ27,
    // размыкающий контакт РУ3, концевики дверей ВВК, параллельные контаты КВВ, РУ4, РУ25
    // При ЭДТ питание получается через провод 737, в обход блокировок тяги
    is_777_ON = ((is_727_ON &&
                   brake_switcher->isTraction() &&
                   kt->getContactState(KT_TRACTION_ALLOW) &&
                   ru27->getContactState(RU27_TRACTION_ALLOW)) || is_737_ON) &&
                 ru3->getContactState(RU3_TRACTION_ALLOW) &&
                 is_DP_closed &&
                 (kvv->getContactState(KVV_SELF_POWER) || ru4->getContactState(RU4_KVG_POWER) || ru25->getContactState(RU25_KVG_POWER));

    ru3->setVoltage(Ucc * static_cast<double>(is_RU3_ON));
    ru3->step(t, dt);

    // Контроль дверей ВВК
    is_DP_closed = (!control_inputs[CABS_NUM][CTRL_VVK_DOOR].toBool()) &&
                   (!control_inputs[CABS_NUM][CTRL_VVK_BLINDS_DOOR].toBool());

    // Цепь реле времени РВ2: в тяге - провод 777;
    bool is_RV2_ON = is_777_ON;

    // Данное реле отключает КП с выдержкой времени 0,8 с, чтобы успели
    // отключиться контакторы КВВ и КВГ, и не загорелась дуга на силовых контактах КП
    rv2->setControlVoltage(Ucc * static_cast<double>(is_RV2_ON));
    rv2->step(t, dt);

    // Цепь провода 800: плюс от автоматов АВ4 "Топливный насос"
    bool is_800_ON = is_AB4_CAB1_on || is_AB4_CAB2_on;

    // Состояние цепи поездных контакторов (КП): плюс от автоматов АВ4, замыкающие контакты РВ2
    is_KP_on = is_800_ON && rv2->getContactState(RV2_KP_ON_OFF);

    tracForce = 0;

    // Состояние последовательной цепи размыкающих контактов КП1 - КП7
    bool is_KP1_KP7_off = true;

    // Состояние последовательной цепи замыкающих контактор КП1 - КП6
    bool is_KP1_KP6_on = true;

    for (size_t i = 0; i < motor.size(); ++i)
    {
        kp[i]->setVoltage(Ucc * static_cast<double>(is_KP_on) * static_cast<double>(tumbler_om[i].getState()));
        kp[i]->step(t, dt);

        // Двигательный режим: напряжение на якорь подается с ВУ главного генератора
        motor[i]->setAncorVoltage(trac_gen->getVoltage() * static_cast<double>(kp[i]->getContactState(KP_POWER_ON)));

        // Генераторный режим: напряжение ВУ главного генератора подается на
        // последовательно соединенные обмотки возбуждения
        motor[i]->setFieldVoltage(trac_gen->getVoltage() * kp[KP7]->getContactState(KP_POWER_ON) / NUM_MOTORS);
        // Якорь замыкается на индивидуальный тормозной резистор
        motor[i]->setBrakeResistorON(kp[i]->getContactState(KP_POWER_ON));

        // Угловая скорость вала она и в Африке угловая скорость вала, в любом режиме
        motor[i]->setOmega(ip * wheel_omega[i]);

        // Ослабление возбуждения
        double beta = 1.0 - 0.42 * static_cast<double>(ksh1->getContactState(0))
                          - 0.24 * static_cast<double>(ksh2->getContactState(0));

        motor[i]->setFieldWeak(beta);

        motor[i]->setReversSate(reversor->getSatate());

        // Режим возбуждения: 1 - последовательное (тяга), -1 - независимое (ЭДТ)
        motor[i]->setMode(brake_switcher->getState() * static_cast<int>(kp[i]->getContactState(KP_POWER_ON)));

        motor[i]->step(t, dt);

        Q_a[i+1] = motor[i]->getTorque() * ip;

        tracForce += Q_a[i+1] * 2 / wheel_diameter[i];

        // В режиме тяги ток через генератор и ВУ - сумма токов, протекающих через якори ТЭД
        I_gen += motor[i]->getAncorCurrent() * static_cast<double>(kp[i]->getContactState(KP_POWER_ON) && kp[KP7]->getContactState(KP_OFF));

        is_KP1_KP7_off = is_KP1_KP7_off && kp[i]->getContactState(KP_OFF);
        is_KP1_KP6_on = is_KP1_KP6_on && (kp[i]->getContactState(KP_ON) || (!tumbler_om[i].getState()));
    }

    // В режиме ЭДТ через генератор и ВУ потребляется ток последовательно соединенных обмоток возбуждения
    // ТЭД. Причем схема ЭДТ не соберется даже при одном неработающем ТЭД, так что учитываем ток
    // лишь одного двигателя, например первого
    I_gen += motor[KP1]->getFieldCurrent() * static_cast<double>(kp[KP7]->getContactState(KP_POWER_ON));

    is_KP1_KP7_off = is_KP1_KP7_off && kp[KP7]->getContactState(KP_OFF);

    // Цепь РУ4 - сбор тяги на 1-й позиции контроллера.
    // Катушка реле получает питание через блокировки от КМ на любой его позиции,
    // кроме первой. Своими нормально замкнутым контактов оно шунтирует контакт
    // самопитания КВВ, позволяя собрать схему тяги ТОЛЬКО на первой позиии
    bool is_RU4_ON1 = KB1_switch->getContactState(KB1_CONTACT_CAB1_K6) &&
                      azv_upr_tepl[CAB1].getState() &&
                      km[CAB1]->isContact(KM_35);

    bool is_RU4_ON2 = KB1_switch->getContactState(KB1_CONTACT_CAB2_K14) &&
                      azv_upr_tepl[CAB2].getState() &&
                      km[CAB1]->isContact(KM_35);

    bool is_RU4_ON = is_RU4_ON1 || is_RU4_ON2;

    ru4->setVoltage(Ucc * static_cast<double>(is_RU4_ON));
    ru4->step(t, dt);

    // Состояние цепи контактора возбуждения генератора
    is_KVG_on = is_777_ON && is_KP1_KP6_on;

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
    bool is_TGF_on = kvg->getContactState(KVG_TRAC_GEN_FIELD_ON);

    trac_gen->setFieldVoltage(field_reg->getFieldVoltage() * static_cast<double>(is_TGF_on));
    trac_gen->setLoadCurrent(I_gen);
    trac_gen->setOmega(disel->getOmega());
    trac_gen->step(t, dt);

    // Активация блока управления возбуждением главного генератора
    field_reg->setActive(kvg->getContactState(KVG_FIELD_REG_ON) && ru24->getContactState(RU24_FIELD_REG_ON));
    // Признак электрического торможения
    field_reg->setStateEDB(ru28->getContactState(RU28_FOR_LS5_ON));
    // Ток задаваемый в режиме ЭДТ
    field_reg->setCurrentRefEDB(edb_control_unit->getRefCurrent());
    field_reg->setFGVoltage(field_gen->getVoltage());
    field_reg->setOmega(disel->getOmega());
    field_reg->setGenVoltage(trac_gen->getVoltage());
    field_reg->setGenCurrent(I_gen);

    // Имитируем переключение питания с КМ по кабинам, в цепях, связанных с регулировкой мощности
    int km_pos_ref = static_cast<double>(KB1_switch->getContactState(KB1_CONTACT_CAB1_K6) * pf(km[CAB1]->getPositionNumber())) +
                     static_cast<double>(KB1_switch->getContactState(KB1_CONTACT_CAB2_K14) * pf(km[CAB2]->getPositionNumber()));

    int km_pos = 0;

    if (button_maneuver[CAB1].getState() || button_maneuver[CAB2].getState())
    {
        // При нажатой кнопке "Маневр" мощность не выше 1-й позиции
        if (km[CAB1]->isZero() && km[CAB2]->isZero())
        {
            km_pos = 1;
        }
        else
        {
            km_pos = km_pos_ref;
        }
    }
    else
    {
        km_pos = km_pos_ref;
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
    is_Revers_Forward = is_710_ON && (km[CAB1]->isForward() || km[CAB2]->isBackward());

    // Цепь вентиля реверсора назад
    is_Revers_Backward = is_710_ON && (km[CAB1]->isBackward() || km[CAB2]->isForward());

    reversor->setForwardValveState(is_Revers_Forward);
    reversor->setBackwardValveState(is_Revers_Backward);
    reversor->step(t, dt);    

    // Цепь вентиля "Тяга" тормозного переключателя
    bool is_TRAC_on = is_819_ON &&
                      ((is_KP1_KP7_off && ru21->getContactState(RU21_TRAC_ON)) ||
                       (brake_switcher->isTraction() && ru21->getContactState(RU21_TRAC_ON)));

    // Цепь вентиля "Торможение" тормозного переключателя
    bool is_BRAKE_on = is_819_ON &&
                       ((is_KP1_KP7_off && ru21->getContactState(RU21_BRAKE_ON)) ||
                        (brake_switcher->isBrake() && ru21->getContactState(RU21_BRAKE_ON)));

    brake_switcher->setTracValeState(is_TRAC_on && kt1->getContactState(KT1_TRACTION_ALLOW));
    brake_switcher->setBrakeValveState(is_BRAKE_on);
    brake_switcher->step(t, dt);
}
