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

    // На проводе 1315 напряжение появляется по цепи: КБ1 в соотвествующем положении, АЗВ ЭДТ в кабине,
    // тублер "Электродинамический тормоз" в ВВК и последовательные контакты отключателей ТЭД (ОМ1-ОМ6).
    // Не учтен рубильник ВкРЗ1
    bool is_1315_ON = ((KB1_switch->getContactState(KB1_CONTACT_CAB1_K8) && azv_edt_on[CAB1].getState()) ||
                       (KB1_switch->getContactState(KB1_CONTACT_CAB2_K16) && azv_edt_on[CAB2].getState())) &&
                      edt_tumbler.getState() && is_OM1_OM6_ON;

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
}
