#include    <tep70.h>

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void TEP70::stepElectroDymanicBrake(const double &t, const double &dt)
{
    // Общая цепь всей схемы реостатного тормоза

    // Цепь отключателей ТЭД
    bool is_OM1_OM6_ON = true;

    for (auto &om : tumbler_om)
    {
        is_OM1_OM6_ON = is_OM1_OM6_ON && om.getState();
    }

    // На проводе 1283 напряжение появляется по цепи: КБ1 в соотвествующем положении, АЗВ ЭДТ в кабине,
    bool is_1283_ON = ((KB1_switch->getContactState(KB1_CONTACT_CAB1_K8) && azv_edt_on[CAB1].getState()) ||
                       (KB1_switch->getContactState(KB1_CONTACT_CAB2_K16) && azv_edt_on[CAB2].getState()));

    // На проводе 1315 напряжение появляется по цепи: от провода 1283,
    // тублер "Электродинамический тормоз" в ВВК и последовательные контакты отключателей ТЭД (ОМ1-ОМ6).
    // Не учтен рубильник ВкРЗ1
    bool is_1315_ON = is_1283_ON &&
                      edt_tumbler.getState() &&
                      is_OM1_OM6_ON;

    // Питание вентиля жалюзи ЭДТ, по сути непосредственно с провода 1315, через ряд разъемов и клемм
    EDB_blinds_actor->setState(is_1315_ON);
    EDB_blinds_actor->step(t, dt);

    // Цепь катушки реле РУ37: от провода 1315 через блок-контакты жалюзи ЭДТ.
    // Реле получает питание после открытия жалюзи ЭДТ
    //
    // Данное реле участвует в создании цепи КВГ и КВВ!
    bool is_RU37_ON = is_1315_ON && EDB_blinds_actor->isOpened();

    ru37->setVoltage(Ucc * static_cast<double>(is_RU37_ON));
    ru37->step(t, dt);

    // Состояние контактов КМ в положении "П"
    bool is_KM_EDB_PREPARE = km[CAB1]->isPrepareEDB() || km[CAB2]->isPrepareEDB();

    // КМ в озне тормозных позиций
    bool is_KM_EDB = km[CAB1]->isEDB() || km[CAB2]->isEDB();

    // Цепь катушки реле РУ26: провод 1283, контакты КМ в положении "П" и далее
    // в любой тормозной позиции
    bool is_RU26_ON = is_1283_ON && is_KM_EDB;

    ru26->setVoltage(Ucc * static_cast<double>(is_RU26_ON));
    ru26->step(t, dt);

    // Цепь катушки реле РУ24: провод 1283, контакты КМ в положении "П", размыкающие контакты реле РУ27
    bool is_RU24_ON = is_1283_ON && is_KM_EDB_PREPARE && ru27->getContactState(RU27_FOR_RU24_ON);

    // Данное реле:
    // 1. Оключает блок управления возбуждением главного генератора
    // 2. Рвет цепь катушек реле РВ10, РВ11, РУ22
    ru24->setVoltage(Ucc * static_cast<double>(is_RU24_ON));
    ru24->step(t, dt);

    // Цепь тормозного контактора: провод 1315, контакты реле РУ26
    bool is_KT_ON = is_1315_ON && ru26->getContactState(RU26_FOR_KT_ON);

    kt->setVoltage(Ucc * static_cast<double>(is_KT_ON));
    kt->step(t, dt);

    // Цепь реле РУ25: провод 1315, параллельные контакты РУ24, РУ26, РУ28, размыкающий контакт РУ23,
    // размыкающий контакт РУ20
    bool is_RU25_ON_FROM_RU24_RU26_RU28 = ru24->getContactState(RU24_FOR_RU25_ON) || ru26->getContactState(RU26_FOR_RU25_ON) || ru28->getContactState(RU28_FOR_RU25_ON);

    bool is_RU23_ON_RU25 = ru23->getContactState(RU23_FOR_RU25_OFF);

    bool is_RU20_ON_RU25 = ru20->getContactState(RU20_FOR_RU25_ON);

    is_RU25_ON = is_1315_ON && is_RU25_ON_FROM_RU24_RU26_RU28 && is_RU23_ON_RU25 && is_RU20_ON_RU25;

    // Данное реле:
    // 1. Шунтирует контакт самопитания катушки КВГ параллельно контакту РУ4
    // 2. Включает реле РУ21
    ru25->setVoltage(Ucc * static_cast<double>(is_RU25_ON));
    ru25->step(t, dt);

    // Цепь реле РУ21: провод 1315, контакты замыкающие реле РУ25,
    // контакты размыкающие датчика давления РДТ2
    bool is_RDT2_ON = (brake_mech[TROLLEY_FWD]->getBCpressure() <= 0.23) &&
                      (brake_mech[TROLLEY_BWD]->getBCpressure() <= 0.23);

    bool is_RU21_ON = is_1315_ON && ru25->getContactState(RU25_FOR_RU21_ON) && is_RDT2_ON;

    ru21->setVoltage(Ucc * static_cast<double>(is_RU21_ON));
    ru21->step(t, dt);

    // Цепь реле РУ22: провод катушки реле РУ21, размыкающий контакт РУ24
    bool is_RU22_ON = is_RU21_ON && ru24->getContactState(RU24_FOR_RU22_ON);

    ru22->setVoltage(Ucc * static_cast<double>(is_RU22_ON));
    ru22->step(t, dt);

    // Цепь реле времени РВ8: провод 728, замыкающие контакты РУ21,
    // замыкающие контакты тормозного контактора КТ
    bool is_RV8_ON = is_728_ON &&
                     ru21->getContactState(RU21_FOR_RV8_ON) &&
                     kt->getContactState(KT_FOR_RV8_ON);

    rv8->setControlVoltage(Ucc * static_cast<double>(is_RV8_ON));
    rv8->step(t, dt);

    // Цепь блока управления электрическим тормозом (БЭТ)
    bool is_EBCU_ON = kt->getContactState(KT_EBCU_ON);

    edb_control_unit->setPowerVoltage(Ucc * static_cast<double>(is_EBCU_ON));

    float selsin_pos = 0.0f;
    double force_level = 0.0;
    bool is_stopping_brake = false;

    if (KB1_switch->getContactState(KB1_CONTACT_CAB1_K6))
    {
        selsin_pos = km[CAB1]->getSelsinPos();
        force_level = brake_force_switch[CAB1].getPosition();
        is_stopping_brake = km[CAB1]->isContact(KM_21);
    }

    if (KB1_switch->getContactState(KB1_CONTACT_CAB2_K14))
    {
        selsin_pos = km[CAB2]->getSelsinPos();
        force_level = brake_force_switch[CAB2].getPosition();
        is_stopping_brake = km[CAB2]->isContact(KM_21);
    }

    edb_control_unit->setVelocitySelsinPos(selsin_pos);
    edb_control_unit->setCurrentVelocity(wheel_omega[KP1] * wheel_diameter[KP1] / 2.0);
    edb_control_unit->setForceLimitLevel(force_level);
    edb_control_unit->fullBrake(rv11->getContactState(RV11_FULL_BRAKE_FORCE));
    edb_control_unit->stoppingBrake(is_stopping_brake);
    edb_control_unit->setAncorCurrent(motor[KP1]->getAncorCurrent());
    edb_control_unit->setFieldCurrent(motor[KP1]->getFieldCurrent());
    edb_control_unit->step(t, dt);

    // Цепь реле РУ32: контроль питания БЭТ
    bool is_RU32_ON = edb_control_unit->isPowered();

    ru32->setVoltage(Ucc * static_cast<double>(is_RU32_ON));
    ru32->step(t, dt);

    // Цепь провода 810: провод 777б замыкающий контакт реле РВ8
    bool is_810_ON = is_777_ON && rv8->getContactState(RV8_KP7_ON_OFF);

    // Катушка контактора КТ1 получает питание от провода 810
    kt1->setVoltage(Ucc * static_cast<double>(is_810_ON));
    kt1->step(t, dt);

    // Цепь катушки поездного контактора КП7: провод 810, замыкающий контакт КТ1
    bool is_KP7_ON = is_810_ON && kt1->getContactState(KT1_FOR_KP7_ON);

    kp[KP7]->setVoltage(Ucc * static_cast<double>(is_KP7_ON));
    kp[KP7]->step(t, dt);

    // Цепь катушки РВ12: провод 1315, замыкающие контакты РУ22
    bool is_RV12_ON = is_1315_ON &&
                      ru22->getContactState(RU22_FOR_RV12_ON);

    // Данное реле активируется при включении РУ22 на тормозных позициях КМ,
    // и через 5 минут непрерывного ЭДТ включает блок-магниты МР1 и МР3,
    // для увеличения оборотов дизеля, соответствующий 14 позиции
    rv12->setControlVoltage(Ucc * static_cast<double>(is_RV12_ON));
    rv12->step(t, dt);

    // Цепь катушки реле РУ28: плюс АКБ (!!!), замыкающие контакты КВГ,
    // размыкающие контакты реле РУ29, замыкающий контакт КП7
    bool is_RU28_ON = kvg->getContactState(KVG_FOR_RU28_ON) &&
                      ru29->getContactState(RU29_FOR_RU28_OFF) &&
                      kp[KP7]->getContactState(KP_ON);

    // Данное реле завершает сбор силовой цепи схемы ЭДТ
    ru28->setVoltage(U_bat * static_cast<double>(is_RU28_ON));
    ru28->step(t, dt);

    // Цепь катушки реле РУ29
    bool is_RU29_ON = false;

    ru29->setVoltage(Ucc * static_cast<double>(is_RU29_ON));
    ru29->step(t, dt);

    // Цепь катушки реле РУ23: провод 1315, размыкающие контакты реле РМн1,
    // замыкающие контакты РВ10
    is_RU23_ON = is_1315_ON &&
                 rmn1->getContactState(RMn1_FOR_RU23_OFF) &&
                 rv10->getContactState(RV10_FOR_RU23_ON);

    // Данное реле рвет цепь катушки реле РУ25, инициируя разбор схемы, если по истечении 8 секунд
    // после сбора схемы ЭДТ ток не достиг 30% максимального (в случае успеха,
    // срабатывает реле РМн1 и не дает РУ23 включиться)
    ru23->setVoltage(Ucc * static_cast<double>(is_RU23_ON));
    ru23->step(t, dt);

    // Цепь катушки РВ10: провод 1315, размыкающие контакты РУ24,
    // замыкающие контакты тормозного контактора КТ
    bool is_RV10_ON = is_1315_ON &&
                      ru24->getContactState(RU24_FOR_RV10_OFF) &&
                      kt->getContactState(KT_FOR_RV10_ON);

    rv10->setControlVoltage(Ucc * static_cast<double>(is_RV10_ON));
    rv10->step(t, dt);

    // Цепь катушки реле времени РВ11: провод 1315, размыкающий контакт РУ24,
    // замыкающий контакт РУ28
    bool is_RV11_ON = is_1315_ON &&
                      ru24->getContactState(RU24_FOR_RV11_OFF) &&
                      ru28->getContactState(RU28_FOR_RV11_ON);

    rv11->setControlVoltage(Ucc * static_cast<double>(is_RV11_ON));
    rv11->step(t, dt);

    // Катушка реле РМн1 получает питание от БЭТ
    rmn1->setVoltage(Ucc * static_cast<double>(edb_control_unit->isReady()));
    rmn1->step(t, dt);

    // Цепь вентиля ВТ1: провод 1315, контакты замыкающие РУ25
    bool is_VT1_ON = is_1315_ON && ru25->getContactState(RU25_FOR_VT1_ON);

    vt1->setVoltage(Ucc * static_cast<double>(is_VT1_ON));
    // Шаг выполняется в томозном оборудовании!!!

    // Цепь вертиля ВТ2
    bool is_VT2_ON = is_stopping_brake && ru28->getContactState(RU28_FOR_VT2_OFF);

    vt2->setVoltage(Ucc * static_cast<double>(is_VT2_ON));
}
