#include    "cooler-fan.h"

#include    <cmath>

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
CoolerFan::CoolerFan(QObject *parent) : Device(parent)
{

}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
CoolerFan::~CoolerFan()
{

}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void CoolerFan::preStep(state_vector_t &Y, double t)
{
    Q_UNUSED(t)

    double omega = Y[OMEGA];

    if (omega > 0.5)
    {
        if (sound_state.state == 0)
        {
            sound_state.state = 1;
        }

        sound_state.volume = 1.0f;
        sound_state.pitch = 0.5f + 1.5f * static_cast<float>(cut(omega / omega_nominal, 0.0, 1.0));
    }
    else
    {
        if (sound_state.state != 0)
        {
            sound_state.state = 0;
        }

        sound_state.volume = 0.0f;
    }
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void CoolerFan::ode_system(const state_vector_t &Y,
                            state_vector_t &dYdt,
                            double t)
{
    Q_UNUSED(t)

    double omega = Y[OMEGA];
    M_resist = K_aero * omega * std::abs(omega);

    dYdt[OMEGA] = (M_drive - M_resist) / J_fan;
    dYdt[ANGLE] = omega;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
double CoolerFan::getResistTorque() const
{
    return M_resist;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
float CoolerFan::getSoundSignal(size_t idx) const
{
    Q_UNUSED(idx)

    return sound_state.createSoundSignal();
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void CoolerFan::load_config(CfgReader &cfg)
{
    QString secName = "Device";

    cfg.getDouble(secName, "K_aero", K_aero);
    cfg.getDouble(secName, "J_fan", J_fan);
    cfg.getDouble(secName, "omega_nominal", omega_nominal);
}