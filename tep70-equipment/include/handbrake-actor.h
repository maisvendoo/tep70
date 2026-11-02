#ifndef     HANDBRAKE_ACTOR_H
#define     HANDBRAKE_ACTOR_H

#include    <device.h>

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
class HandbrakeActor : public Device
{
public:

    HandbrakeActor(QObject *parent = Q_NULLPTR);

    ~HandbrakeActor();

    void setShaftAngle(double shaft_angle)
    {
        this->shaft_angle = shaft_angle;
    }

    double getForceLevel() const
    {
        return force_level;
    }

private:

    /// Уровень усилия прижатия колодок (0...1.0)
    double force_level = 0;

    /// Угол поворота вала штурвала (в относительных единицах 0.0..1.0)
    double shaft_angle = 0;

    /// Зазор в механизме привода, в долях относительного угла
    double gap = 0.0;

    /// Коэффциент передачи привода
    double K_force = 1.0;

    void preStep(state_vector_t &Y, double t) override;

    void ode_system(const state_vector_t &Y,
                    state_vector_t &dYdt,
                    double t) override;

    void load_config(CfgReader &cfg) override;
};

#endif
