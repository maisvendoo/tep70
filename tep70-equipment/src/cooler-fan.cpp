#include    "cooler-fan.h"

#include    <cmath>

CoolerFan::CoolerFan(QObject *parent) : Device(parent)
{

}

CoolerFan::~CoolerFan()
{

}

void CoolerFan::preStep(state_vector_t &Y, double t)
{
    Q_UNUSED(Y)
    Q_UNUSED(t)

    M_resist = K_aero * omega * std::abs(omega);
}

void CoolerFan::ode_system(const state_vector_t &Y, state_vector_t &dYdt, double t)
{
    Q_UNUSED(Y)
    Q_UNUSED(dYdt)
    Q_UNUSED(t)
}

void CoolerFan::load_config(CfgReader &cfg)
{
    QString secName = "Device";

    cfg.getDouble(secName, "K_aero", K_aero);
}