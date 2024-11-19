#include    "tep70bs.h"

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void TEP70BS::stepSafetyDevices(double t, double dt)
{
    // Приёмные катушки АЛСН
    coil_ALSN_fwd->step(t, dt);
    coil_ALSN_bwd->step(t, dt);

    // Дешифратор АЛСН
    alsn_decoder->setCoilSignal(coil_ALSN_fwd->getCode());
    alsn_decoder->step(t, dt);

    // Электропневматический клапан автостопа
    epk->setFLpressure(main_reservoir->getPressure());
    epk->setBPpressure(brakepipe->getPressure());
    epk->setPowered(true);
    epk->setControl(keys);
    epk->step(t, dt);
}
