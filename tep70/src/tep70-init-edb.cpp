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

    ru26 = new Relay(NUM_RU26_CONTACTS);
    ru26->read_config("mk-6", custom_cfg_dir);
    ru26->setInitContactState(RU26_FOR_RU25_ON, false);
    ru26->setInitContactState(RU26_FOR_KT_ON, false);

    ru27 = new Relay(NUM_RU27_CONTACTS);
    ru27->setInitContactState(RU27_FOR_RU24_ON, true);
    ru27->read_config("mk-6", custom_cfg_dir);

    ru24 = new Relay(NUM_RU24_CONTACTS);
    ru24->read_config("mk-6", custom_cfg_dir);
    ru24->setInitContactState(RU24_FIELD_REG_ON, true);
    ru24->setInitContactState(RU24_FOR_RU25_ON, false);

    ru23 = new Relay(NUM_RU23_CONTACTS);
    ru23->read_config("mk-6", custom_cfg_dir);
    ru23->setInitContactState(RU23_FOR_RU25_ON, true);

    ru20 = new Relay(NUM_RU20_CONTACTS);
    ru20->read_config("mk-6", custom_cfg_dir);
    ru20->setInitContactState(RU20_FOR_RU25_ON, true);

    ru25 = new Relay(NUM_RU25_CONTACTS);
    ru25->read_config("mk-6", custom_cfg_dir);
    ru25->setInitContactState(RU25_KVG_POWER, false);
    ru25->setInitContactState(RU25_FOR_RU21_ON, false);

    kt = new Relay(NUM_KT_CONTACTS);
    kt->read_config("mk-6", custom_cfg_dir);
    kt->setInitContactState(KT_RESET_POWER, true);
    kt->setInitContactState(KT_TRACTION_ALLOW, true);

    ru21->read_config("mk-6", custom_cfg_dir);
    ru21->setInitContactState(RU21_TRAC_ON, true);
    ru21->setInitContactState(RU21_BRAKE_ON, false);
}
