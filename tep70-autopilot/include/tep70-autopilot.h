#ifndef     TEP70_AUTOPILOT_H
#define     TEP70_AUTOPILOT_H

#include    <autopilot.h>
#include    <tep70-autopilot-types.h>

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
class TEP70Autopilot : public Autopilot
{
public:

    TEP70Autopilot();

    ~TEP70Autopilot();

    auto_control_t *getControl() override;

    void step(double t, double dt) override;

    void initAutoBrakeControl(const QString& config_name,
                              const QString& custom_cfg_dir) override;

private:

    double Imax = 0.0;

    double delta_I = 0.0;

    double Kp = 1.0;

    double Ki = 0.0;

    double Ks = 1.0;

    double dv = 0.0;

    bool lock_traction = false;

    /// Выдержка рукоятки КМ
    const double KM_DELAY = 2.0;

    /// Таймер выдержки главной рукоятки КМ в заданном
    Timer *km_delay = new Timer(KM_DELAY, false);

    tep70_control_t *auto_control = new tep70_control_t();

    tep70_feedback_t *auto_feedback = nullptr;

    void preStep(state_vector_t &Y, double t) override;

    void ode_system(const state_vector_t &Y,
                    state_vector_t &dYdt,
                    double t) override;

    void load_config(CfgReader &cfg) override;

    void plusPos();

    void minusPos();

    void zeroPos();

private slots:

    void slotDelayTimer();
};

#endif
