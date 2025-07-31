#ifndef     CABINE_SWITCHER_H
#define     CABINE_SWITCHER_H

#include    <device.h>

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
class CabineSwitcher : public Device
{
public:

    CabineSwitcher(QObject *parent = nullptr);

    ~CabineSwitcher();

    void setCabineIndex(int cabine_idx)
    {
        this->cabine_idx = cabine_idx;
    }

    bool isCabine1() const
    {
        return is_cabine1;
    }

    bool isCabine2() const
    {
        return is_cabine2;
    }

private:

    /// Индекс текущей кабины
    int cabine_idx = 0;

    /// Кабина 1 активна
    bool is_cabine1 = false;

    /// Кабина 2 активна
    bool is_cabine2 = false;

    void preStep(state_vector_t &Y, double t);

    void ode_system(const state_vector_t &Y, state_vector_t &dYdt, double t);

    void load_config(CfgReader &cfg);
};

#endif
