#ifndef     COOLER_FAN_H
#define     COOLER_FAN_H

#include    "device.h"

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
class CoolerFan : public Device
{
public:

    CoolerFan(QObject *parent = Q_NULLPTR);

    ~CoolerFan();

    /// Задать крутящий момент от привода
    void setDriveTorque(double M) { M_drive = M; }

    /// Угловая скорость крыльчатки
    double getFanOmega() const { return getY(OMEGA); }

    /// Угол поворота для анимации
    double getAngle() const { return getY(ANGLE); }

    /// Момент аэродинамического сопротивления
    double getResistTorque() const;

    /// Звуковой сигнал
    float getSoundSignal(size_t idx = 0) const override;

private:

    enum
    {
        OMEGA = 0,
        ANGLE = 1
    };

    /// Момент привода
    double M_drive = 0.0;
    /// Момент сопротивления
    double M_resist = 0.0;
    /// Коэффциент аэродинамического сопротивления
    double K_aero = 0.02;
    /// Момент инерции вентилятора
    double J_fan = 5.0;

    /// Номинальная частота для нормализации высоты звука
    double omega_nominal = 70.0;

    /// Состояние звука
    sound_state_t sound_state;

    void preStep(state_vector_t &Y, double t) override;

    void ode_system(const state_vector_t &Y,
                    state_vector_t &dYdt,
                    double t) override;

    void load_config(CfgReader &cfg) override;
};

#endif // COOLER_FAN_H