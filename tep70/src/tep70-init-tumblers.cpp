#include    "tep70.h"

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------
void TEP70::initControl(const QString &modules_dir, const QString &custom_cfg_dir)
{
    (void) modules_dir;
    (void) custom_cfg_dir;

    // Расцепные рычаги и концевые краны управляются от IOController (shared)
    // в stepControls. Клавиши и мышь заданы в конфиге iocontroller.

    // АЗВ "Компрессор" — управление от IOController (shared), клавиша в конфиге iocontroller
    //azv_motor_compressor.setInitState(true);

    for (auto cab_idx : {CAB1, CAB2})
    {
        // Тумблер "Ослабление поля I ступени руч./авт." (Тб 1)
        tumbler_field_weak1[cab_idx].setNumPositions(3);
        tumbler_field_weak1[cab_idx].setInitPosition(1);

        // Тумблер "Ослабление поля II ступени руч./авт." (Тб 2)
        tumbler_field_weak2[cab_idx].setNumPositions(3);
        tumbler_field_weak2[cab_idx].setInitPosition(1);

        // Тумблер "Управление жалюзи воды руч./авт." (Тб 8)
        tumbler_water_zaluzi[cab_idx].setNumPositions(3);
        tumbler_water_zaluzi[cab_idx].setInitPosition(1);

        // Тумблер "Управление жалюзи масла руч./авт." (Тб 9)
        tumbler_oil_zaluzi[cab_idx].setNumPositions(3);
        tumbler_oil_zaluzi[cab_idx].setInitPosition(1);

        // Тумблер "Аварийная остановка дизеля" (Тб 6)
        tumbler_disel_stop[cab_idx].setInitState(true);

        // Тумблер "Буферный фонарь левый" (Тб 35)
        tumbler_bufferliht_L[cab_idx].setNumPositions(3);
        tumbler_bufferliht_L[cab_idx].setInitPosition(1);

        // Тумблер "Буферный фонарь правый" (Тб 36)
        tumbler_bufferliht_R[cab_idx].setNumPositions(3);
        tumbler_bufferliht_R[cab_idx].setInitPosition(1);

        // Переключатель тормозного усилия
        brake_force_switch[cab_idx].setNumPositions(12);
        brake_force_switch[cab_idx].setInitPosition(0);

        // Диагностика: принудительный сброс кнопок
        button_check_edt[cab_idx].reset();
        button_check_fire_alarm[cab_idx].reset();
        button_empty2[cab_idx].reset();
    }
}
