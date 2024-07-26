#include    "tep70bs.h"

#include    <QDir>

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void TEP70BS::initElectroTransmission(const QString &modules_dir, const QString &custom_cfg_dir)
{
    (void) modules_dir;

    // Возбудитель главного генератора
    field_gen = new FieldGenerator();

    kvv = new Relay(1);
    kvv->read_config("mk-6", custom_cfg_dir);
    kvv->setInitContactState(0, false);

    kvg = new Relay(3);
    kvg->read_config("mk-6", custom_cfg_dir);
    kvg->setInitContactState(0, false);
    kvg->setInitContactState(1, true);
    kvg->setInitContactState(2, false);

    trac_gen = new TracGenerator();
    trac_gen->read_config("trac-generator", custom_cfg_dir);
    trac_gen->load_marnetic_char(custom_cfg_dir + QDir::separator() + "gs-504a.txt");
    trac_gen->load_eff_coeff(custom_cfg_dir + QDir::separator() + "gs-504a-eff_coeff.txt");

    field_reg = new FieldRegulator();
    field_reg->read_config("field-regulator", custom_cfg_dir);
    field_reg->load_settings(custom_cfg_dir + QDir::separator() + "field-reg-settings.txt");

    for (size_t i = 0; i < motor.size(); ++i)
    {
        motor[i] = new TractionMotor();
        motor[i]->read_config("ed-119", custom_cfg_dir);
        motor[i]->load_magnetic_char(custom_cfg_dir + QDir::separator() + "ed-119.txt");
        motor[i]->load_eff_coeff(custom_cfg_dir + QDir::separator() + "ed-119-eff_coeff.txt");

        kp[i] = new Relay(3);
        kp[i]->read_config("mk-6", custom_cfg_dir);
        kp[i]->setInitContactState(0, false);
        kp[i]->setInitContactState(1, true);
        kp[i]->setInitContactState(2, false);
    }

    kp[6] = new Relay(3);
    kp[6]->read_config("mk-6", custom_cfg_dir);
    kp[6]->setInitContactState(0, false);
    kp[6]->setInitContactState(1, true);
    kp[6]->setInitContactState(2, false);

    //speed_meter = new SL2M();
    //speed_meter->read_config("3SL-2M", custom_cfg_dir);

    ksh1 = new Relay(3);
    ksh1->read_config("mk-6", custom_cfg_dir);
    ksh1->setInitContactState(0, false);
    ksh1->setInitContactState(1, false);
    ksh1->setInitContactState(2, false);

    ksh2 = new Relay(3);
    ksh2->read_config("mk-6", custom_cfg_dir);
    ksh2->setInitContactState(0, false);
    ksh2->setInitContactState(1, false);
    ksh2->setInitContactState(2, true);

    ru1 = new Relay(3);
    ru1->read_config("mk-6", custom_cfg_dir);
    ru1->setInitContactState(0, false);
    ru1->setInitContactState(1, false);
    ru1->setInitContactState(2, false);

    reversor = new Reversor();
    reversor->read_config("reversor", custom_cfg_dir);

    brake_switcher = new BrakeSwitcher();
    brake_switcher->read_config("brake-switcher", custom_cfg_dir);

    rp1 = new HysteresisRelay(0.079, 0.162);
    rp2 = new HysteresisRelay(0.079, 0.162);

    ksh2_delay = new TimeRelay(1);
    ksh2_delay->read_config("rpu-3m", custom_cfg_dir);
    ksh2_delay->setTimeoutOn(2.0);
    ksh2_delay->setInitContactState(0, false);

    ksh1_delay = new TimeRelay(1);
    ksh1_delay->read_config("rpu-3m", custom_cfg_dir);
    ksh1_delay->setTimeoutOn(2.0);
    ksh1_delay->setInitContactState(0, true);
}
