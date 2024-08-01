#include    "tep70.h"

#include    <QDir>

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void TEP70::initDisel(const QString &modules_dir, const QString &custom_cfg_dir)
{
    (void) modules_dir;

    disel = new Disel();
    disel->read_config("disel", custom_cfg_dir);

    starter_generator = new StarterGenerator();
    starter_generator->read_config("starter-generator", custom_cfg_dir);

    starter_generator->init(custom_cfg_dir + QDir::separator() + "5sg-magnetic-char.txt");

    voltage_regulator = new VoltageRegulator();
    voltage_regulator->read_config("voltage-regulator", custom_cfg_dir);
}
