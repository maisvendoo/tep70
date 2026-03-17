#include    <tep70-autopilot.h>

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
TEP70Autopilot::TEP70Autopilot() : Autopilot(nullptr)
{
    connect(km_delay, &Timer::process, this, &TEP70Autopilot::slotDelayTimer);
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
TEP70Autopilot::~TEP70Autopilot()
{

}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
auto_control_t *TEP70Autopilot::getControl()
{
    return auto_control;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void TEP70Autopilot::step(double t, double dt)
{
    Autopilot::step(t, dt);

    km_delay->step(t, dt);
    brake_control->step(t, dt);
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void TEP70Autopilot::initAutoBrakeControl(const QString &config_name,
                                          const QString &custom_cfg_dir)
{
    brake_control->read_config(config_name, custom_cfg_dir);
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void TEP70Autopilot::preStep(state_vector_t &Y, double t)
{
    // Приводим общую структуру обратной связи к нашему типу
    auto_feedback = dynamic_cast<tep70_feedback_t *>(feedback);

    if (auto_feedback == nullptr)
    {
        return;
    }

    Y[0] = cut(Y[0], -1.0, 1.0);

    dv = v_ref - auto_feedback->v_cur;

    // Вычисляем задание по току ТЭД
    double kp = Kp * train_mass / ref_mass;

    double dv_s = pf(feedback->v_tau - feedback->v_cur);

    double I_ref = Imax * (kp * dv - Ks * dv_s + getY(0));

    I_ref = cut(I_ref, 0.0, Imax);

    // Блокирование тяги по давлению в ТЦ
    if (auto_feedback->pBC > 0.04)
    {
        lock_traction = true;
        zeroPos();
    }
    else
    {
        // Если тяга заблокирована но скорость не упала сильно
        if (lock_traction && dv < 10.0)
            lock_traction = true; // продолжаем блокировать тягу
        else
            lock_traction = false;
    }

    if (qAbs(I_ref) < 100.0)
    {
        zeroPos();
    }

    // Если ток упал ниже уставки
    if (auto_feedback->I_gen < I_ref - delta_I)
    {
        if (!lock_traction)
        {
            // + позиция
            plusPos();
        }
    }

    // Если ток сильно выше уставки
    if (auto_feedback->I_gen > I_ref + delta_I)
    {
        // - позиция
        minusPos();
    }

    // Если превышаем скорость - мотаем вниз до упора
    if (dv < -dV_traction_off)
    {
        zeroPos();
    }

    brake_control->setBrakePressures(auto_feedback->pEQ,
                                     auto_feedback->pBC,
                                     auto_feedback->p_charge);

    brake_control->setFeedback(auto_feedback->v_cur, dist_target, a_brake, accel_meter->value());

    brake_control->step_control(auto_feedback->is_EPB_on,
                                dv,
                                is_motion_allowed,
                                lock_traction,
                                is_disable_release);

    autopilot_brake_control_state_t bc_state = brake_control->getControlState();

    auto_control->krm_pos = bc_state.brake_crane_pos_ref;
    auto_control->kvt_pos = bc_state.loco_crane_pos_ref;

    // Управляем прожектором - включаем когда разрешено движение
    auto_control->spotlight_ON = is_motion_allowed;

    // Отбиваем ПСС
    auto_control->press_RB = auto_feedback->is_vigilance_control;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void TEP70Autopilot::ode_system(const state_vector_t &Y,
                                state_vector_t &dYdt,
                                double t)
{
    dYdt[0] = Ki * dv;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void TEP70Autopilot::load_config(CfgReader &cfg)
{
    Autopilot::load_config(cfg);

    QString secName = "Device";

    cfg.getDouble(secName, "Imax", Imax);
    cfg.getDouble(secName, "DeltaI", delta_I);
    cfg.getDouble(secName, "Kp", Kp);
    cfg.getDouble(secName, "Ki", Ki);
    cfg.getDouble(secName, "Ks", Ks);
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void TEP70Autopilot::plusPos()
{
    if (km_delay->isStarted())
    {
        return;
    }

    auto_control->km_pos++;

    if (auto_control->km_pos > 15)
    {
        auto_control->km_pos = 15;
    }

    if (!km_delay->isStarted())
    {
        km_delay->start();
    }
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void TEP70Autopilot::minusPos()
{
    if (km_delay->isStarted())
    {
        return;
    }

    auto_control->km_pos--;

    if (auto_control->km_pos < 0)
    {
        auto_control->km_pos = 0;
    }

    if (!km_delay->isStarted())
    {
        km_delay->start();
    }
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void TEP70Autopilot::zeroPos()
{
    if (km_delay->isStarted())
    {
        km_delay->stop();
        return;
    }

    auto_control->km_pos = 0;

    if (!km_delay->isStarted())
    {
        km_delay->start();
    }
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void TEP70Autopilot::slotDelayTimer()
{
    km_delay->stop();
}

GET_AUTOPILOT(TEP70Autopilot)
