#include    <tep70.h>
#include    <train-horn.h>

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void TEP70::stepAutopilot(double t, double dt)
{
    if (autopilot.empty())
    {
        return;
    }

    double v_lim = 0;
    double v_lim_next = 0;
    double limit_dist = 0;
    double signal_dist = 0;
    ALSN alsn_code = ALSN::NO_CODE;

    int cab_idx = 0;

    if (km[CAB1]->isReversHandle())
    {
        v_lim = speedmap_fwd->getCurrentLimit();
        v_lim_next = speedmap_fwd->getNextLimit();
        limit_dist = speedmap_fwd->getNextLimitDistance();
        alsn_code = coil_ALSN_fwd->getCode();
        signal_dist = coil_ALSN_fwd->getNextSignalDistance();

        cab_idx = CAB1;
    }

    if (km[CAB2]->isReversHandle())
    {
        v_lim = speedmap_bwd->getCurrentLimit();
        v_lim_next = speedmap_bwd->getNextLimit();
        limit_dist = speedmap_bwd->getNextLimitDistance();
        alsn_code = coil_ALSN_bwd->getCode();
        signal_dist = coil_ALSN_bwd->getNextSignalDistance();

        cab_idx = CAB2;
    }

    if (autopilot[cab_idx] == nullptr)
    {
        return;
    }

    // Включение и выключение автоведения
    autopilot_switcher[cab_idx].getState() ? autopilot[cab_idx]->on() : autopilot[cab_idx]->off();

    // Сигнал контроля бдительности от цепей ламп ПСС
    auto_feedback[cab_idx]->is_vigilance_control = static_cast<bool>(safety_device[cab_idx]->getStatePSS());
    // Текущая позиция ЭКГ
    auto_feedback[cab_idx]->I_gen = I_gen;
    auto_feedback[cab_idx]->v_cur = qAbs(velocity * Physics::kmh);
    auto_feedback[cab_idx]->v_tau = qAbs(wheel_omega[0] * wheel_diameter[0] / 2.0 * Physics::kmh);
    auto_feedback[cab_idx]->v_lim = v_lim;
    auto_feedback[cab_idx]->v_lim_next = v_lim_next;
    auto_feedback[cab_idx]->limit_dist = limit_dist;
    auto_feedback[cab_idx]->alsn_code = alsn_code;
    auto_feedback[cab_idx]->signal_dist = signal_dist;
    auto_feedback[cab_idx]->pBC = brake_mech[TROLLEY_FWD]->getBCpressure();
    auto_feedback[cab_idx]->pEQ = brake_crane[cab_idx]->getERpressure();
    auto_feedback[cab_idx]->p_charge = charge_press;
    auto_feedback[cab_idx]->is_EPB_on = epb_control->stateReleaseLamp();


    // Принимаем сигналы обратной связи от оборудования
    autopilot[cab_idx]->setFeedback(auto_feedback[cab_idx]);

    // Выполняем шаг управления
    autopilot[cab_idx]->step(t, dt);

    // Получаем управляющие воздействия
    auto_control[cab_idx] = dynamic_cast<tep70_control_t *>(autopilot[cab_idx]->getControl());

    // Действия по управлению, только если автоведение активно
    if (autopilot[cab_idx]->isActive())
    {
        // Проверка бдительности
        auto_control[cab_idx]->press_RB ? rb[cab_idx][RBS].set() : rb[cab_idx][RBS].reset();

        // Управление КМ
        km[cab_idx]->setPos(auto_control[cab_idx]->km_pos);

        // Управление КрМ
        brake_crane[cab_idx]->setHandlePosition(auto_control[cab_idx]->krm_pos);

        // Управление КВТ
        loco_crane[cab_idx]->setHandlePosition(auto_control[cab_idx]->kvt_pos);

        // Управление прожектором
        auto_control[cab_idx]->spotlight_ON ? azv_spotlight_low[cab_idx].set() :
            azv_spotlight_low[cab_idx].reset();

        sand_system->setSandDeliveryOn(auto_control[cab_idx]->sand_ON);

        horn->lockManualControl(true);
        horn->setSvistokOn(auto_control[cab_idx]->whistle);
        horn->setTifonOn(auto_control[cab_idx]->typhoid);
    }
    else
    {
        horn->lockManualControl(false);
    }
}
