#include    "tep70.h"

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------
void TEP70::initEPB(const QString &modules_dir, const QString &custom_cfg_dir)
{
    (void) modules_dir;
    (void) custom_cfg_dir;

    for (auto cab_idx : {CAB1, CAB2})
    {
        // Преобразователь напряжения для ЭПТ
        epb_converter[cab_idx] = new EPBConverter();
        epb_converter[cab_idx]->read_config("epb-converter-110V");

        // Контроллер двухпроводного ЭПТ
        epb_control[cab_idx] = new EPBControl();
    }
}
