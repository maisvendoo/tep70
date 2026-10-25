#ifndef     COOLER_FAN_H
#define     COOLER_FAN_H

#include    "device.h"

class CoolerFan : public Device
{
public:

    CoolerFan(QObject *parent = Q_NULLPTR);

    ~CoolerFan();

    /// Задать крутящий момент от гидропривода
    void setDriveTorque(double M) { M_drive = M; }

    /// Угловая скорость крыльчатки
    double getFanOmega() const { return getY(0); }

    /// Угол поворота для анимации
    double getAngle() const { return getY(ANGLE); }

    /// Момент аэродинамического сопротивления
    double getResistTorque() const;

private:

    enum
    {
        OMEGA = 0,
        ANGLE = 1
    };

    double M_drive = 0.0;
    double M_resist = 0.0;
    double K_aero = 0.02;
    double J_fan = 5.0;

    void preStep(state_vector_t &Y, double t) override;
    void ode_system(const state_vector_t &Y, state_vector_t &dYdt, double t) override;
    void load_config(CfgReader &cfg) override;
};

#endif // COOLER_FAN_H