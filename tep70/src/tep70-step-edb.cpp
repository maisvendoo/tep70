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

    // Цепь реле РУ25: провод 1315, параллельные контакты РУ24 и РУ26, размыкающий контакт РУ23,
    // размыкающий контакт РУ20
    bool is_RU25_ON = is_1315_ON &&
                     (ru24->getContactState(RU24_FOR_RU25_ON) || ru26->getContactState(RU26_FOR_RU25_ON)) &&
                     ru23->getContactState(RU23_FOR_RU25_ON) &&
                     ru20->getContactState(RU20_FOR_RU25_ON);

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
}
