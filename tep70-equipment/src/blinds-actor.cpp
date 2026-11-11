#include    "blinds-actor.h"

#include    <algorithm>

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
BlindsActor::BlindsActor(QObject *parent) : Device(parent)
{

}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
BlindsActor::~BlindsActor()
{

}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void BlindsActor::step(double t, double dt)
{
    bool prev = isOpened();

    Device::step(t, dt);

    bool curr = isOpened();

    if (prev != curr)
    {
        if (curr)
        {
            sound_state_open.play(true);
            sound_timer_open = 0.5;
        }
        else
        {
            sound_state_close.play(true);
            sound_timer_close = 0.5;
        }
    }

    sound_timer_open = std::max(0.0, sound_timer_open - dt);
    sound_timer_close = std::max(0.0, sound_timer_close - dt);
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void BlindsActor::preStep(state_vector_t &Y, double t)
{
    Q_UNUSED(t)

    cur_rate = is_open ? max_rate : -max_rate;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void BlindsActor::ode_system(const state_vector_t &Y,
                              state_vector_t &dYdt,
                              double t)
{
    Q_UNUSED(t)

    dYdt[0] = cur_rate;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
float BlindsActor::getSoundSignal(size_t idx) const
{
    if (idx == 0)
    {
        if (sound_timer_open > 0.0)
            return sound_state_open.createSoundSignal();
    }
    else
    {
        if (sound_timer_close > 0.0)
            return sound_state_close.createSoundSignal();
    }

    return sound_state_t().createSoundSignal();
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void BlindsActor::load_config(CfgReader &cfg)
{
    QString secName = "Device";

    cfg.getDouble(secName, "max_rate", max_rate);
}