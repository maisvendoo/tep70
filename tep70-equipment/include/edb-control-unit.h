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
        return If_ref;
    }

    void setCurrentVelocity(double v)
    {
        Vkmh = qAbs(v) * Physics::kmh;
    }

    void setRefVelocity(double v)
    {
        this->V_ref = v;
    }

    void setAncorCurrent(double Ia)
    {
        this->Ia = Ia;
    }

    void setFieldCurrent(double If)
    {
        this->If = If;
    }

private:

    double Ucc = 0.0;

    bool is_power = false;

    /// Заданный ток возбуждения ТЭД
    double If_ref = 0.0;

    double Vmax = 160.0;

    double Ia_max = 825.0;

    double If_max = 800.0;

    double Vkmh = 0.0;

    double V_ref = 0.0;

    double Ia = 0.0;

    double If = 0.0;

    double eps_I = 0.0;

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
