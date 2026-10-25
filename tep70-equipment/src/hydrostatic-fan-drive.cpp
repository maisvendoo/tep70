#include    "hydrostatic-fan-drive.h"

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
HydrostaticFanDrive::HydrostaticFanDrive(QObject *parent) : Device(parent)
{

}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
HydrostaticFanDrive::~HydrostaticFanDrive()
{

}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void HydrostaticFanDrive::preStep(state_vector_t &Y, double t)
{
    Q_UNUSED(t)

    if (Y[0] < 0.001)
        Y[0] = 0.0;

    double omega_ideal = V0_nom * Y[0] * disel_omega * gear_ratio * eta_vol;

    if (V_motor > 1.0e-12)
        omega_ideal /= V_motor;

    double M_max = p_relief * V_motor / (2.0 * Physics::PI);
    M_drive = cut(K_hyd * (omega_ideal - fan_omega), -M_max, M_max);
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void HydrostaticFanDrive::ode_system(const state_vector_t &Y, state_vector_t &dYdt, double t)
{
    Q_UNUSED(t)

    dYdt[0] = (gamma_target - Y[0]) / tau;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void HydrostaticFanDrive::load_config(CfgReader &cfg)
{
    QString secName = "Device";

    cfg.getDouble(secName, "V0_nom", V0_nom);
    cfg.getDouble(secName, "V_motor", V_motor);
    cfg.getDouble(secName, "eta_vol", eta_vol);
    cfg.getDouble(secName, "tau", tau);
    cfg.getDouble(secName, "gear_ratio", gear_ratio);
    cfg.getDouble(secName, "K_hyd", K_hyd);
    cfg.getDouble(secName, "p_relief", p_relief);
}