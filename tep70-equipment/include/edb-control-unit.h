#ifndef     EDB_CONTROL_UNIT
#define     EDB_CONTROL_UNIT

#include    <device.h>

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
class ControlUnitEDB : public Device
{
public:

    ControlUnitEDB(QObject *parent = nullptr);

    ~ControlUnitEDB();

    void setPowerVoltage(double Ucc)
    {
        this->Ucc = Ucc;
    }

    bool isPowered() const
    {
        return is_power;
    }

    double getRefCurrent() const
    {
        return u_ref;//getY(1);
    }

    void setCurrentVelocity(double v)
    {
        Vkmh = qAbs(v) * Physics::kmh;
    }    

    void setAncorCurrent(double Ia)
    {
        this->Ia = Ia;
    }

    void setFieldCurrent(double If)
    {
        this->If = If;
    }

    void step(double t, double dt) override;

    void setVelocitySelsinPos(float selsin_pos)
    {
        velosity_selsin_pos = static_cast<double>(selsin_pos);
    }

    float getRefVelocityKmh() const
    {
        return static_cast<float>(V_ref);
    }

    void setForceLimitLevel(double force_limit_level)
    {
        this->force_limit_level = force_limit_level;
    }

    void fullBrake(bool is_full_brake)
    {
        this->is_full_brake = is_full_brake;
    }

    bool isReady() const
    {
        return is_brake_ready;
    }

private:

    double Ucc = 0.0;

    bool is_power = false;

    /// Заданный ток возбуждения ТЭД
    double u_ref = 0.0;

    double Vmax = 160.0;

    double Ia_max = 825.0;

    double If_max = 800.0;

    double Vkmh = 0.0;

    double V_ref = 0.0;

    double Ia = 0.0;

    double If = 0.0;

    double eps_I = 0.0;

    double eps_I_prev = 0.0;

    double deps_Idt = 0.0;

    double integrator_lim = 1.0;

    double velosity_selsin_pos = 0.0;

    double force_limit_level = 0.0;

    double min_force_level = 0.22;

    double force_level_step = 0.071;

    double force_prep_level = 0.2;

    bool is_full_brake = false;

    double min_brake_current_level = 0.3;

    bool is_brake_ready = false;

    /// Скорость начала ограничения тока якоря по условиям коммутации
    double Vc = 115.0;

    /// Максимальная заданная скорость торможения
    double V_ref_max = 150.0;

    enum
    {
        NUM_COEFS = 10
    };

    std::array<double, NUM_COEFS>   K;

    void preStep(state_vector_t &Y, double t) override;

    void ode_system(const state_vector_t &Y,
                    state_vector_t &dYdt,
                    double t) override;

    void load_config(CfgReader &cfg) override;
};

#endif
