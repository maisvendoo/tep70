#ifndef     HYDROSTATIC_FAN_DRIVE_H
#define     HYDROSTATIC_FAN_DRIVE_H

#include    "device.h"

class HydrostaticFanDrive : public Device
{
public:

    HydrostaticFanDrive(QObject *parent = Q_NULLPTR);

    ~HydrostaticFanDrive();

    void setWaterTemp(double T) { T_water = T; }

    void setDiselOmega(double omega) { disel_omega = omega; }

    double getFanOmega() const;

private:

    double T_water = 20.0;
    double disel_omega = 0.0;

    double V0_nom = 1.0e-6;
    double V_motor = 1.0e-6;
    double eta_vol = 0.9;
    double gamma_min = 0.05;
    double gamma_max = 1.0;
    double T_min = 70.0;
    double T_max = 92.0;
    double hysteresis = 3.0;
    double tau = 15.0;
    double gear_ratio = 1.05;

    double gamma_target = 0.0;

    void preStep(state_vector_t &Y, double t);

    void ode_system(const state_vector_t &Y, state_vector_t &dYdt, double t);

    void load_config(CfgReader &cfg);
};

#endif // HYDROSTATIC_FAN_DRIVE_H