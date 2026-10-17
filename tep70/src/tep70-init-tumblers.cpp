#include    "tep70.h"

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------
void TEP70::initControl(const QString &modules_dir, const QString &custom_cfg_dir)
{
    (void) modules_dir;
    (void) custom_cfg_dir;

    // Расцепные рычаги
    oper_rod_fwd->setKeySymbol(KEY_X);

    oper_rod_bwd->setKeySymbol(KEY_C);

    // Концевые краны и рукава управляются от IOController (shared) в stepControls.
    // Клавиши заданы в konфиге iocontroller.

    // АЗВ "Компрессор" — управление от IOController (shared), клавиша в konфиге iocontroller
    azv_motor_compressor.setInitState(true);

    for (auto cab_idx : {CAB1, CAB2})
    {
        // АЗВ "Освещение кабины" (АВ 26)
        azv_cabine_light[cab_idx].setKeySymbolOn(KEY_K);
        azv_cabine_light[cab_idx].setKeyModifierOn(MODIFIER_OnlyShift);
        azv_cabine_light[cab_idx].setKeySymbolOff(KEY_K);
        azv_cabine_light[cab_idx].setKeyModifierOff(MODIFIER_OnlyControl);

        // АЗВ "Освещение пульта" (АВ 25)
        azv_panel_light[cab_idx].setKeySymbolOn(KEY_L);
        azv_panel_light[cab_idx].setKeyModifierOn(MODIFIER_OnlyShift);
        azv_panel_light[cab_idx].setKeySymbolOff(KEY_L);
        azv_panel_light[cab_idx].setKeyModifierOff(MODIFIER_OnlyControl);

        // АЗВ "Управление общее" (АВ 2)
        azv_common_control[cab_idx].setKeySymbolOn(KEY_U);
        azv_common_control[cab_idx].setKeyModifierOn(MODIFIER_OnlyShift);
        azv_common_control[cab_idx].setKeySymbolOff(KEY_U);
        azv_common_control[cab_idx].setKeyModifierOff(MODIFIER_OnlyControl);

        // АЗВ "Управление тепловозом" (АВ 1)
        azv_upr_tepl[cab_idx].setKeySymbolOn(KEY_I);
        azv_upr_tepl[cab_idx].setKeyModifierOn(MODIFIER_OnlyShift);
        azv_upr_tepl[cab_idx].setKeySymbolOff(KEY_I);
        azv_upr_tepl[cab_idx].setKeyModifierOff(MODIFIER_OnlyControl);

        // АЗВ "Топливный насос" (АВ 4)
        azv_fuel_pump[cab_idx].setKeySymbolOn(KEY_P);
        azv_fuel_pump[cab_idx].setKeyModifierOn(MODIFIER_OnlyShift);
        azv_fuel_pump[cab_idx].setKeySymbolOff(KEY_P);
        azv_fuel_pump[cab_idx].setKeyModifierOff(MODIFIER_OnlyControl);

        // АЗВ "ЭДТ" (АВ 7)
        azv_edt_on[cab_idx].setKeySymbolOn(KEY_F);
        azv_edt_on[cab_idx].setKeyModifierOn(MODIFIER_OnlyShift);
        azv_edt_on[cab_idx].setKeySymbolOff(KEY_F);
        azv_edt_on[cab_idx].setKeyModifierOff(MODIFIER_OnlyControl);

        // АЗВ "ЭПТ" (АВ 19)
        azv_ept_on[cab_idx].setKeySymbolOn(KEY_V);
        azv_ept_on[cab_idx].setKeyModifierOn(MODIFIER_OnlyShift);
        azv_ept_on[cab_idx].setKeySymbolOff(KEY_V);
        azv_ept_on[cab_idx].setKeyModifierOff(MODIFIER_OnlyControl);

        // АЗВ "ЭПТ питание" (АВ 18)
        azv_ept_power[cab_idx].setKeySymbolOn(KEY_V);
        azv_ept_power[cab_idx].setKeyModifierOn(MODIFIER_OnlyAlt);
        azv_ept_power[cab_idx].setKeySymbolOff(KEY_V);
        azv_ept_power[cab_idx].setKeyModifierOff(MODIFIER_OnlyAlt);

        // АЗВ "Прожектор тускло" (АВ 23)
        azv_spotlight_low[cab_idx].setKeySymbolOn(KEY_H);
        azv_spotlight_low[cab_idx].setKeyModifierOn(MODIFIER_OnlyShift);
        azv_spotlight_low[cab_idx].setKeySymbolOff(KEY_H);
        azv_spotlight_low[cab_idx].setKeyModifierOff(MODIFIER_OnlyControl);

        // АЗВ "Прожектор ярко" (АВ 24)
        azv_spotlight_high[cab_idx].setKeySymbolOn(KEY_H);
        azv_spotlight_high[cab_idx].setKeyModifierOn(MODIFIER_OnlyAlt);
        azv_spotlight_high[cab_idx].setKeySymbolOff(KEY_H);
        azv_spotlight_high[cab_idx].setKeyModifierOff(MODIFIER_OnlyAlt);

        // Тумблер "Ослабление поля I ступени руч./авт." (Тб 1)
        tumbler_field_weak1[cab_idx].setNumPositions(3);
        tumbler_field_weak1[cab_idx].setKeySymbolIncrease(KEY_1);
        tumbler_field_weak1[cab_idx].setKeyModifierIncrease(MODIFIER_OnlyShift);
        tumbler_field_weak1[cab_idx].setKeySymbolDecrease(KEY_1);
        tumbler_field_weak1[cab_idx].setKeyModifierDecrease(MODIFIER_OnlyControl);
        tumbler_field_weak1[cab_idx].setInitPosition(1);

        // Тумблер "Ослабление поля II ступени руч./авт." (Тб 2)
        tumbler_field_weak2[cab_idx].setNumPositions(3);
        tumbler_field_weak2[cab_idx].setKeySymbolIncrease(KEY_2);
        tumbler_field_weak2[cab_idx].setKeyModifierIncrease(MODIFIER_OnlyShift);
        tumbler_field_weak2[cab_idx].setKeySymbolDecrease(KEY_2);
        tumbler_field_weak2[cab_idx].setKeyModifierDecrease(MODIFIER_OnlyControl);
        tumbler_field_weak2[cab_idx].setInitPosition(1);

        // Тумблер "Управление жалюзи воды руч./авт." (Тб 8)
        tumbler_water_zaluzi[cab_idx].setNumPositions(3);
        tumbler_water_zaluzi[cab_idx].setKeySymbolIncrease(KEY_3);
        tumbler_water_zaluzi[cab_idx].setKeyModifierIncrease(MODIFIER_OnlyShift);
        tumbler_water_zaluzi[cab_idx].setKeySymbolDecrease(KEY_3);
        tumbler_water_zaluzi[cab_idx].setKeyModifierDecrease(MODIFIER_OnlyControl);
        tumbler_water_zaluzi[cab_idx].setInitPosition(1);

        // Тумблер "Управление жалюзи масла руч./авт." (Тб 9)
        tumbler_oil_zaluzi[cab_idx].setNumPositions(3);
        tumbler_oil_zaluzi[cab_idx].setKeySymbolIncrease(KEY_4);
        tumbler_oil_zaluzi[cab_idx].setKeyModifierIncrease(MODIFIER_OnlyShift);
        tumbler_oil_zaluzi[cab_idx].setKeySymbolDecrease(KEY_4);
        tumbler_oil_zaluzi[cab_idx].setKeyModifierDecrease(MODIFIER_OnlyControl);
        tumbler_oil_zaluzi[cab_idx].setInitPosition(1);

        // Тумблер "Напряжение ЦУ. Напряжение ЭПТ" (Тб 15)
        tumbler_voltage[cab_idx].setKeySymbolOn(KEY_9);
        tumbler_voltage[cab_idx].setKeyModifierOn(MODIFIER_OnlyShift);
        tumbler_voltage[cab_idx].setKeySymbolOff(KEY_9);
        tumbler_voltage[cab_idx].setKeyModifierOff(MODIFIER_OnlyControl);

        // Тумблер "Аварийная остановка дизеля" (Тб 6)
        tumbler_disel_stop[cab_idx].setKeySymbolOn(KEY_O);
        tumbler_disel_stop[cab_idx].setKeyModifierOn(MODIFIER_OnlyShift);
        tumbler_disel_stop[cab_idx].setKeySymbolOff(KEY_O);
        tumbler_disel_stop[cab_idx].setKeyModifierOff(MODIFIER_OnlyControl);
        tumbler_disel_stop[cab_idx].setInitState(true);

        // Тумблер "Буферный фонарь левый" (Тб 35)
        tumbler_bufferliht_L[cab_idx].setNumPositions(3);
        tumbler_bufferliht_L[cab_idx].setKeySymbolIncrease(KEY_G);
        tumbler_bufferliht_L[cab_idx].setKeyModifierIncrease(MODIFIER_OnlyShift);
        tumbler_bufferliht_L[cab_idx].setKeySymbolDecrease(KEY_G);
        tumbler_bufferliht_L[cab_idx].setKeyModifierDecrease(MODIFIER_OnlyControl);
        tumbler_bufferliht_L[cab_idx].setInitPosition(1);

        // Тумблер "Буферный фонарь правый" (Тб 36)
        tumbler_bufferliht_R[cab_idx].setNumPositions(3);
        tumbler_bufferliht_R[cab_idx].setKeySymbolIncrease(KEY_J);
        tumbler_bufferliht_R[cab_idx].setKeyModifierIncrease(MODIFIER_OnlyShift);
        tumbler_bufferliht_R[cab_idx].setKeySymbolDecrease(KEY_J);
        tumbler_bufferliht_R[cab_idx].setKeyModifierDecrease(MODIFIER_OnlyControl);
        tumbler_bufferliht_R[cab_idx].setInitPosition(1);

        // Тумблер "Номерные знаки" (Тб 37)
        tumbler_number_light[cab_idx].setKeySymbolOn(KEY_Backslash);
        tumbler_number_light[cab_idx].setKeyModifierOn(MODIFIER_OnlyShift);
        tumbler_number_light[cab_idx].setKeySymbolOff(KEY_Backslash);
        tumbler_number_light[cab_idx].setKeyModifierOff(MODIFIER_OnlyControl);

        // Тумблер "Подсветка пульта помощника"
        tumbler_panel_pom_light[cab_idx].setKeySymbolOn(KEY_6);
        tumbler_panel_pom_light[cab_idx].setKeyModifierOn(MODIFIER_OnlyShift);
        tumbler_panel_pom_light[cab_idx].setKeySymbolOff(KEY_6);
        tumbler_panel_pom_light[cab_idx].setKeyModifierOff(MODIFIER_OnlyControl);

        // Тумблер "Резервный свет"
        tumbler_reserve_light[cab_idx].setKeySymbolOn(KEY_7);
        tumbler_reserve_light[cab_idx].setKeyModifierOn(MODIFIER_OnlyShift);
        tumbler_reserve_light[cab_idx].setKeySymbolOff(KEY_7);
        tumbler_reserve_light[cab_idx].setKeyModifierOff(MODIFIER_OnlyControl);

        // Тумблер "Калорифер кабины"
        tumbler_cab_heater[cab_idx].setKeySymbolOn(KEY_8);
        tumbler_cab_heater[cab_idx].setKeyModifierOn(MODIFIER_OnlyShift);
        tumbler_cab_heater[cab_idx].setKeySymbolOff(KEY_8);
        tumbler_cab_heater[cab_idx].setKeyModifierOff(MODIFIER_OnlyControl);

        // Тумблер "Вентилятор кабины"
        tumbler_cab_fan[cab_idx].setKeySymbolOn(KEY_0);
        tumbler_cab_fan[cab_idx].setKeyModifierOn(MODIFIER_OnlyShift);
        tumbler_cab_fan[cab_idx].setKeySymbolOff(KEY_0);
        tumbler_cab_fan[cab_idx].setKeyModifierOff(MODIFIER_OnlyControl);

        // Кнопка "Пуск дизеля" (Кн 3)
        button_disel_start[cab_idx].setKeySymbolOn(KEY_P);
        button_disel_start[cab_idx].setKeyModifierOn(MODIFIER_OnlyAlt);
        button_disel_start[cab_idx].setKeySymbolOff(KEY_Undefined);
        button_disel_start[cab_idx].setKeyModifierOff(KEY_Undefined);

        // Кнопка "Отпуск тормозов" (Кн 17)
        button_brake_release[cab_idx].setKeySymbolOn(KEY_Slash);
        button_brake_release[cab_idx].setKeyModifierOn(ANY_MODIFIERS);
        button_brake_release[cab_idx].setKeySymbolOff(KEY_Undefined);
        button_brake_release[cab_idx].setKeyModifierOff(KEY_Undefined);

        // Кнопка "Свисток" (Кн 7)
        button_svistok[cab_idx].setKeySymbolOn(KEY_Space);
        button_svistok[cab_idx].setKeyModifierOn(ANY_MODIFIERS);
        button_svistok[cab_idx].setKeySymbolOff(KEY_Undefined);
        button_svistok[cab_idx].setKeyModifierOff(KEY_Undefined);

        // Кнопка "Тифон" (Кн 6)
        button_tifon[cab_idx].setKeySymbolOn(KEY_B);
        button_tifon[cab_idx].setKeyModifierOn(ANY_MODIFIERS);
        button_tifon[cab_idx].setKeySymbolOff(KEY_Undefined);
        button_tifon[cab_idx].setKeyModifierOff(KEY_Undefined);

        // Триггеры рукояток бдительности
        rb[cab_idx][RBS].setKeySymbolOn(KEY_M);
        rb[cab_idx][RBS].setKeyModifierOn(ANY_MODIFIERS);
        rb[cab_idx][RBS].setKeySymbolOff(KEY_Undefined);
        rb[cab_idx][RBS].setKeyModifierOff(KEY_Undefined);

        rb[cab_idx][RB].setKeySymbolOn(KEY_Z);
        rb[cab_idx][RB].setKeyModifierOn(ANY_MODIFIERS);
        rb[cab_idx][RB].setKeySymbolOff(KEY_Undefined);
        rb[cab_idx][RB].setKeyModifierOff(KEY_Undefined);

        rb[cab_idx][RBP].setKeySymbolOn(KEY_Tilde);
        rb[cab_idx][RBP].setKeyModifierOn(ANY_MODIFIERS);
        rb[cab_idx][RBP].setKeySymbolOff(KEY_Undefined);
        rb[cab_idx][RBP].setKeyModifierOff(KEY_Undefined);

        // Переключатель тормозного усилия
        brake_force_switch[cab_idx].setNumPositions(12);
        brake_force_switch[cab_idx].setKeySymbolIncrease(KEY_Period);
        brake_force_switch[cab_idx].setKeyModifierIncrease(ANY_MODIFIERS);
        brake_force_switch[cab_idx].setKeySymbolDecrease(KEY_Comma);
        brake_force_switch[cab_idx].setKeyModifierDecrease(ANY_MODIFIERS);
        brake_force_switch[cab_idx].setInitPosition(0);

        shunting_mode_switcher[cab_idx].setKeySymbolOn(KEY_Tilde);
        shunting_mode_switcher[cab_idx].setKeyModifierOn(MODIFIER_OnlyShift);
        shunting_mode_switcher[cab_idx].setKeySymbolOff(KEY_Tilde);
        shunting_mode_switcher[cab_idx].setKeyModifierOff(MODIFIER_OnlyControl);
    }
}
