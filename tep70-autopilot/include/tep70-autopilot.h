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

    tep70_control_t *auto_control = new tep70_control_t();

    tep70_feedback_t *auto_feedback = nullptr;

    void preStep(state_vector_t &Y, double t) override;

    void load_config(CfgReader &cfg) override;
};

#endif
