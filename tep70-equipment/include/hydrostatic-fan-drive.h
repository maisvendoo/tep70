#ifndef     HYDROSTATIC_FAN_DRIVE_H
#define     HYDROSTATIC_FAN_DRIVE_H

#include    "device.h"

class HydrostaticFanDrive : public Device
{
public:

    HydrostaticFanDrive(QObject *parent = Q_NULLPTR);

    ~HydrostaticFanDrive();

    void setSwashPlate(double gamma) { gamma_target = gamma; }

    void setDiselOmega(double omega) { disel_omega = omega; }

    void setFanOmega(double omega) { fan_omega = omega; }

    double getDriveTorque() const { return M_drive; }

    double getGamma() const { return getY(0); }

private:

    double disel_omega = 0.0;
    double fan_omega = 0.0;
    double M_drive = 0.0;
    double gamma_target = 0.0;

    double V0_nom = 1.0e-6;
    double V_motor = 1.0e-6;
    double eta_vol = 0.9;
    double tau = 15.0;
    double gear_ratio = 1.05;
    double K_hyd = 100.0;
    double p_relief = 2.0e7;

    void preStep(state_vector_t &Y, double t) override;

    void ode_system(const state_vector_t &Y, state_vector_t &dYdt, double t);

    void load_config(CfgReader &cfg);
};

#endif // HYDROSTATIC_FAN_DRIVE_H