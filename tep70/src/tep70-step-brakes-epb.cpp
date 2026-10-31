#include    "tep70.h"

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------
void TEP70::stepEPB(const double& t, const double& dt)
{
    // Потребляемый ток электровоздухораспределителя
    double evr_current = electro_air_dist->getCurrent(0);

    // Потребляемый ток в рабочей линии ЭПТ
    double epb_work_curr = 0.0;
    epb_work_curr += evr_current;
    epb_work_curr += hose_bp_fwd->getCurrent(0);
    epb_work_curr += hose_bp_bwd->getCurrent(0);

    bool is_KB2_on[CABS_NUM] = {false, false};
    is_KB2_on[CAB1] = KB2_switch->getContactState(KB2_CONTACT_CAB1_K5);
    is_KB2_on[CAB2] = KB2_switch->getContactState(KB2_CONTACT_CAB2_K13);

    for (auto cab_idx : {CAB1, CAB2})
    {
        // Преобразователь напряжения для ЭПТ
        bool is_epb_power = azv_ept_power[cab_idx].getState();
        epb_converter[cab_idx]->setInputVoltage(Ucc * static_cast<double>(is_epb_power));
        epb_converter[cab_idx]->setOutputCurrent(epb_work_curr);
        epb_converter[cab_idx]->step(t, dt);

        // Контроллер двухпроводного ЭПТ
        bool cab_on = is_KB2_on[cab_idx] && brake_lock[cab_idx]->isStateOn() && azv_ept_on[cab_idx].getState();

        epb_control[cab_idx]->setInputVoltage(epb_converter[cab_idx]->getOutputVoltage() * static_cast<double>(cab_on));

        epb_control[cab_idx]->setHoldState(cab_on && brake_crane[cab_idx]->isHold());

        epb_control[cab_idx]->setBrakeState(cab_on && brake_crane[cab_idx]->isBrake());

        epb_control[cab_idx]->setControlVoltage(hose_bp_fwd->getVoltage(1) + hose_bp_bwd->getVoltage(1));
        epb_control[cab_idx]->step(t, dt);
    }

    double epb_work_U = epb_control[CAB1]->getWorkVoltage() + epb_control[CAB2]->getWorkVoltage();
    double epb_work_f = epb_control[CAB1]->getWorkFrequency() + epb_control[CAB2]->getWorkFrequency();

    // Управление электровоздухораспределителем: отключается кнопкой "Отпуск тормозов",
    // а иначе задаём управляющий сигнал из контроллера или рабочей линии ЭПТ
    double evr_U = 0.0;
    double evr_f = 0.0;
    if (!(button_brake_release[CAB1].getState() || button_brake_release[CAB2].getState()))
    {
        evr_U = epb_work_U + hose_bp_fwd->getVoltage(0) + hose_bp_bwd->getVoltage(0);
        evr_f = epb_work_f + hose_bp_fwd->getFrequency(0) + hose_bp_bwd->getFrequency(0);
    }
    electro_air_dist->setVoltage  (0, evr_U);
    electro_air_dist->setFrequency(0, evr_f);

    // Межвагонные сигналы линий ЭПТ по рукавам тормозной магистрали
    // Рабочая линия спереди
    hose_bp_fwd->setVoltage  (0, hose_bp_bwd->getVoltage(0) + epb_work_U);
    hose_bp_fwd->setFrequency(0, hose_bp_bwd->getFrequency(0) + epb_work_f);
    hose_bp_fwd->setCurrent  (0, hose_bp_bwd->getCurrent(0) + evr_current);
    // Контрольная линия спереди
    hose_bp_fwd->setVoltage  (1, hose_bp_bwd->getVoltage(1));
    hose_bp_fwd->setFrequency(1, hose_bp_bwd->getFrequency(1));
    hose_bp_fwd->setCurrent  (1, hose_bp_bwd->getCurrent(1));

    // Рабочая линия сзади
    hose_bp_bwd->setVoltage  (0, hose_bp_fwd->getVoltage(0) + epb_work_U);
    hose_bp_bwd->setFrequency(0, hose_bp_fwd->getFrequency(0) + epb_work_f);
    hose_bp_bwd->setCurrent  (0, hose_bp_fwd->getCurrent(0) + evr_current);
    // Контрольная линия сзади
    hose_bp_bwd->setVoltage  (1, hose_bp_fwd->getVoltage(1));
    hose_bp_bwd->setFrequency(1, hose_bp_fwd->getFrequency(1));
    hose_bp_bwd->setCurrent  (1, hose_bp_fwd->getCurrent(1));
}
