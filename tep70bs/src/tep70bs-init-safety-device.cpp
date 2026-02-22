#include    <tep70bs.h>
#include    <QDir>

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void TEP70BS::initSafetyDevices(const QString& modules_dir, const QString& custom_cfg_dir)
{
    (void) modules_dir;

    // Карта ограничений скорости
    speedmap_fwd = new SpeedMap();
    speedmap_fwd->setDirection(1);
    addRailwayConnector(speedmap_fwd, length / 2.0);

    speedmap_bwd = new SpeedMap();
    speedmap_bwd->setDirection(-1);
    addRailwayConnector(speedmap_bwd, -length / 2.0);

    // Приёмные катушки АЛСН
    coil_ALSN_fwd = new CoilALSN();
    coil_ALSN_fwd->setDirection(1);
    addRailwayConnector(coil_ALSN_fwd, length / 2.0);

    coil_ALSN_bwd = new CoilALSN();
    coil_ALSN_bwd->setDirection(-1);
    addRailwayConnector(coil_ALSN_bwd, -length / 2.0);

    for (size_t cab_idx : {CAB1, CAB2})
    {
        // ЭПК автостопа
        epk[cab_idx] = loadAutoTrainStop(modules_dir + QDir::separator() + "epk150");
        epk[cab_idx]->read_config("epk150");

        // Дешифратор АЛСН
        alsn_decoder[cab_idx] = new DecoderALSN();
        alsn_decoder[cab_idx]->read_config("ALSN-decoder");
    }
}
