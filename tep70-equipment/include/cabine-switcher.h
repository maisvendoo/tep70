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

private:

    void preStep(state_vector_t &Y, double t);

    void ode_system(const state_vector_t &Y, state_vector_t &dYdt, double t);

    void load_config(CfgReader &cfg);
};

#endif
