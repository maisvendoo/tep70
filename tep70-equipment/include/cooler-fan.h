#ifndef     COOLER_FAN_H
#define     COOLER_FAN_H

#include    "device.h"

class CoolerFan : public Device
{
public:

    CoolerFan(QObject *parent = Q_NULLPTR);

    ~CoolerFan();

    void setFanOmega(double omega) { this->omega = omega; }

    double getResistTorque() const { return M_resist; }

    double getFanOmega() const { return omega; }

private:

    double  K_aero = 0.02;
    double  omega = 0.0;
    double  M_resist = 0.0;

    void preStep(state_vector_t &Y, double t) override;
    void ode_system(const state_vector_t &Y, state_vector_t &dYdt, double t) override;
    void load_config(CfgReader &cfg) override;
};

#endif // COOLER_FAN_H