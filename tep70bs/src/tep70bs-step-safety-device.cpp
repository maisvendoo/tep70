#include    "tep70bs.h"

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void TEP70BS::stepSafetyDevices(const double& t, const double& dt)
{
    // Приёмные катушки АЛСН
    coil_ALSN_fwd->step(t, dt);
    coil_ALSN_bwd->step(t, dt);

    // Дешифратор АЛСН
    alsn_decoder[CAB1]->setCoilSignal(coil_ALSN_fwd->getCode());
    alsn_decoder[CAB1]->step(t, dt);

    alsn_decoder[CAB2]->setCoilSignal(coil_ALSN_bwd->getCode());
    alsn_decoder[CAB2]->step(t, dt);

    for (size_t cab_idx : {CAB1, CAB2})
    {
        // Электропневматический клапан автостопа
        epk[cab_idx]->setFLpressure(main_reservoir->getPressure());
        epk[cab_idx]->setBPpressure(brakepipe->getPressure());
        epk[cab_idx]->setPowered(true);
        epk[cab_idx]->step(t, dt);
    }
}
