#include    "tep70bs.h"

#include    <QDir>

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void TEP70BS::initControlCircuit(const QString &modules_dir, const QString &custom_cfg_dir)
{
    (void) modules_dir;

    battery = new Battery();
    battery->read_config("battery", custom_cfg_dir);

    tumbler_disel_stop.set();

    kontaktor_fuel_pump = new Relay(2);
    kontaktor_fuel_pump->read_config("mk-6", custom_cfg_dir);
    kontaktor_fuel_pump->setInitContactState(0, false);
    kontaktor_fuel_pump->setInitContactState(1, false);

    ru8 = new Relay(4);
    ru8->read_config("rpu-3m", custom_cfg_dir);
    ru8->setInitContactState(0, false);
    ru8->setInitContactState(1, false);
    ru8->setInitContactState(2, false);
    ru8->setInitContactState(3, false);

    kontaktor_oil_pump = new Relay(2);
    kontaktor_oil_pump->read_config("mk-6", custom_cfg_dir);
    kontaktor_oil_pump->setInitContactState(0, false);
    kontaktor_oil_pump->setInitContactState(1, false);

    oilpump_time_relay = new TimeRelay(3);
    oilpump_time_relay->read_config("rpu-3m", custom_cfg_dir);
    oilpump_time_relay->setTimeout(60.0);
    oilpump_time_relay->setInitContactState(0, false);
    oilpump_time_relay->setInitContactState(1, false);
    oilpump_time_relay->setInitContactState(2, false);

    kontaktor_starter = new Relay(2);
    kontaktor_starter->read_config("mk-6", custom_cfg_dir);
    kontaktor_starter->setInitContactState(0, false);
    kontaktor_starter->setInitContactState(1, false);

    starter_time_relay = new TimeRelay(1);
    starter_time_relay->read_config("rpu-3m", custom_cfg_dir);
    starter_time_relay->setTimeout(12.0);
    starter_time_relay->setInitContactState(0, false);

    ru10 = new Relay(2);
    ru10->read_config("rpu-3m", custom_cfg_dir);
    ru10->setInitContactState(0, false);
    ru10->setInitContactState(1, false);

    ru6 = new Relay(4);
    ru6->read_config("rpu-3m", custom_cfg_dir);
    ru6->setInitContactState(0, false);
    ru6->setInitContactState(1, true);
    ru6->setInitContactState(2, false);
    ru6->setInitContactState(3, true);

    ru42 = new Relay(3);
    ru42->read_config("rpu-3m", custom_cfg_dir);
    ru42->setInitContactState(0, true);
    ru42->setInitContactState(1, false);

    mv6 = new Relay(1);
    mv6->read_config("rpu-3m", custom_cfg_dir);
    mv6->setInitContactState(0, false);

    vtn = new Relay(1);
    vtn->read_config("rpu-3m", custom_cfg_dir);
    vtn->setInitContactState(0, true);

    ru4 = new Relay(1);
    ru4->read_config("rpu-3m", custom_cfg_dir);
    ru4->setInitContactState(0, true);

    ru15 = new Relay(3);
    ru15->read_config("rpu-3m", custom_cfg_dir);
    ru15->setInitContactState(0, false);
    ru15->setInitContactState(1, false);
    ru15->setInitContactState(2, false);

    rv4 = new TimeRelay(1);
    rv4->read_config("rpu-3m", custom_cfg_dir);
    rv4->setTimeout(60.0);
    rv4->setInitContactState(0, true);

    rv9 = new TimeRelay(1);
    rv9->read_config("rpu-3m", custom_cfg_dir);
    rv9->setTimeout(5.0);
    rv9->setInitContactState(0, false);

    krn = new Relay(6);
    krn->read_config("mk-6", custom_cfg_dir);
    krn->setInitContactState(0, false);
    krn->setInitContactState(1, false);
    krn->setInitContactState(2, true);
    krn->setInitContactState(3, true);
    krn->setInitContactState(4, false);
    krn->setInitContactState(5, false);
}
