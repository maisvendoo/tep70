#include    "tep70bs.h"

#include    <QDir>

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void TEP70BS::initCabineControls(const QString& modules_dir, const QString& custom_cfg_dir)
{
    (void) modules_dir;

    for (auto i : {CAB1, CAB2})
    {
        km[i] = new ControllerKM2202();
        km[i]->read_config("km-2202", custom_cfg_dir);
    }
}
