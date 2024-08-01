#ifndef     SL2M_H
#define     SL2M_H

#include    "device.h"

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
class SL2M : public Device
{
public:

    SL2M(QObject *parent = Q_NULLPTR);

    ~SL2M();

    void setOmega(double value);

    void setWheelDiameter(double diam);

    float getArrowPos() const;

    float getShaftPos() const;

    float getSoundSignal(size_t idx = 0) const override
    {
        (void) idx;
        return sound_state_t::createSoundSignal(abs(omega) >= omega_begin_sound);
    }

private:

    /// Угловая скорость вращения колесной пары
    double omega;

    /// Передаточное число червячного редуктора
    double ip;

    /// Угловая скорость вращения вала
    double omega_s;

    /// Показатель износта - величина разбега стрелки, км/ч
    double wear_gap;

    /// Максимальна скорость на шкале
    double max_speed;

    /// Сигнал для анимации положения стрелки
    float arrow_pos;

    /// Диаметр бандажа
    double Dk = 1.2;

    double sound_speed;

    /// Сигнал положения вала
    float shaft_pos;

    double freq_coeff;

    double speed_begin_sound = 2.0;

    double omega_begin_sound = speed_begin_sound * 2.0 / Dk / Physics::kmh;

    void preStep(state_vector_t &Y, double t);

    void ode_system(const state_vector_t &Y, state_vector_t &dYdt, double t);

    void load_config(CfgReader &cfg);
};

#endif // SL2M_H
