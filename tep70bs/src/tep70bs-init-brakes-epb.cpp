#include    "tep70bs.h"

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------
void TEP70BS::initEPB(const QString &modules_dir, const QString &custom_cfg_dir)
{
    (void) modules_dir;
    (void) custom_cfg_dir;

    // Преобразователь напряжения для ЭПТ
    epb_converter = new EPBConverter();
    epb_converter->read_config("epb-converter-110V");

    // Контроллер двухпроводного ЭПТ
    epb_control = new EPBControl();
}
