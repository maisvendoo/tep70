#include    <tep70.h>

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void TEP70::initElectrodynamicBrake(const QString &modules_dir,
                                    const QString &custom_cfg_dir)
{
    // Привод жалюзи ЭДТ
    EDB_blinds_actor = new BlindsActor;
    EDB_blinds_actor->read_config("edb-blinds-actor", custom_cfg_dir);

    ru37 = new Relay(NUM_RU37_CONTACTS);
    ru37->read_config("mk-6", custom_cfg_dir);
    ru37->setInitContactState(RU37_EDB_BLINGS_LAMP, false);
}
