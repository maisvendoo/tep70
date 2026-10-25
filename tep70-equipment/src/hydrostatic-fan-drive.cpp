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

    if (Y[0] < 1.0)
        Y[0] = 0.0;

    if (T_water >= T_max)
    {
        gamma_target = gamma_max;
    }
    else if (T_water <= T_min - hysteresis)
    {
        gamma_target = gamma_min;
    }
    else if (T_water >= T_min)
    {
        gamma_target = gamma_min + (gamma_max - gamma_min) *
                                    (T_water - T_min) / (T_max - T_min);
    }

    double gamma = Y[0];
    double Q_pump = V0_nom * gamma * disel_omega * gear_ratio;
    double Q_real = Q_pump * eta_vol;

    double omega_motor = 0.0;
    if (V_motor > 1.0e-12)
        omega_motor = Q_real / V_motor;

    double p = p_nom * gamma;
    if (omega_motor > 1.0)
        p += M_load * omega_motor / (V_motor * 1.0e6);

    M_drive = p * V_motor / (2.0 * Physics::PI);
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void HydrostaticFanDrive::ode_system(const state_vector_t &Y, state_vector_t &dYdt, double t)
{
    Q_UNUSED(t)

    double gamma = Y[0];
    dYdt[0] = (gamma_target - gamma) / tau;
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
    cfg.getDouble(secName, "gamma_min", gamma_min);
    cfg.getDouble(secName, "gamma_max", gamma_max);
    cfg.getDouble(secName, "T_min", T_min);
    cfg.getDouble(secName, "T_max", T_max);
    cfg.getDouble(secName, "hysteresis", hysteresis);
    cfg.getDouble(secName, "tau", tau);
    cfg.getDouble(secName, "gear_ratio", gear_ratio);
    cfg.getDouble(secName, "p_nom", p_nom);
}