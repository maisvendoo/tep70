#include    "blinds-actor.h"

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
sound_state_t BlindsActor::getSoundState(size_t idx) const
{
    if (idx == OPEN_SOUND)
    {
        return sound_state_open;
    }

    if (idx == CLOSE_SOUND)
    {
        return sound_state_close;
    }

    return sound_state_t();
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void BlindsActor::preStep(state_vector_t &Y, double t)
{
    Q_UNUSED(t)

    cur_rate = is_open ? max_rate : -max_rate;

    Y[0] = cut(Y[0], 0.0, 1.0);

    if (is_open != is_open_prev)
    {
        if (is_open)
        {
            sound_state_open.play(true);
        }
        else
        {
            sound_state_close.play(true);
        }
    }
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
    if (idx == OPEN_SOUND)
    {
        return sound_state_open.createSoundSignal();
    }

    if (idx == CLOSE_SOUND)
    {
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

    cfg.getDouble(secName, "MaxRate", max_rate);
}
