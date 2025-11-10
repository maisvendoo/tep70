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
    oper_rod_fwd->setControl(&pressed_keys);

    oper_rod_bwd->setKeySymbol(KEY_C);
    oper_rod_bwd->setControl(&pressed_keys);

    // Концевые краны магистрали тормозных цилиндров
    anglecock_bc_fwd->setKeySymbolOpen(KEY_F10);
    anglecock_bc_fwd->setKeyModifierOpen(MODIFIER_OnlyShift);
    anglecock_bc_fwd->setKeySymbolClose(KEY_F10);
    anglecock_bc_fwd->setKeyModifierClose(MODIFIER_OnlyControl);
    anglecock_bc_fwd->setControl(&pressed_keys);

    anglecock_bc_bwd->setKeySymbolOpen(KEY_F11);
    anglecock_bc_bwd->setKeyModifierOpen(MODIFIER_OnlyShift);
    anglecock_bc_bwd->setKeySymbolClose(KEY_F11);
    anglecock_bc_bwd->setKeyModifierClose(MODIFIER_OnlyControl);
    anglecock_bc_bwd->setControl(&pressed_keys);

    // Рукава магистрали тормозных цилиндров
    hose_bc_fwd->setKeySymbolConnect(KEY_F9);
    hose_bc_fwd->setKeyModifierConnect(MODIFIER_OnlyShift);
    hose_bc_fwd->setKeySymbolDisconnect(KEY_F9);
    hose_bc_fwd->setKeyModifierDisconnect(MODIFIER_OnlyControl);
    hose_bc_fwd->setControl(&pressed_keys);

    hose_bc_bwd->setKeySymbolConnect(KEY_F12);
    hose_bc_bwd->setKeyModifierConnect(MODIFIER_OnlyShift);
    hose_bc_bwd->setKeySymbolDisconnect(KEY_F12);
    hose_bc_bwd->setKeyModifierDisconnect(MODIFIER_OnlyControl);
    hose_bc_bwd->setControl(&pressed_keys);

    // Концевые краны питательной магистрали
    anglecock_fl_fwd->setKeySymbolOpen(KEY_F6);
    anglecock_fl_fwd->setKeyModifierOpen(MODIFIER_OnlyShift);
    anglecock_fl_fwd->setKeySymbolClose(KEY_F6);
    anglecock_fl_fwd->setKeyModifierClose(MODIFIER_OnlyControl);
    anglecock_fl_fwd->setControl(&pressed_keys);

    anglecock_fl_bwd->setKeySymbolOpen(KEY_F7);
    anglecock_fl_bwd->setKeyModifierOpen(MODIFIER_OnlyShift);
    anglecock_fl_bwd->setKeySymbolClose(KEY_F7);
    anglecock_fl_bwd->setKeyModifierClose(MODIFIER_OnlyControl);
    anglecock_fl_bwd->setControl(&pressed_keys);

    // Рукава питательной магистрали
    hose_fl_fwd->setKeySymbolConnect(KEY_F5);
    hose_fl_fwd->setKeyModifierConnect(MODIFIER_OnlyShift);
    hose_fl_fwd->setKeySymbolDisconnect(KEY_F5);
    hose_fl_fwd->setKeyModifierDisconnect(MODIFIER_OnlyControl);
    hose_fl_fwd->setControl(&pressed_keys);

    hose_fl_bwd->setKeySymbolConnect(KEY_F8);
    hose_fl_bwd->setKeyModifierConnect(MODIFIER_OnlyShift);
    hose_fl_bwd->setKeySymbolDisconnect(KEY_F8);
    hose_fl_bwd->setKeyModifierDisconnect(MODIFIER_OnlyControl);
    hose_fl_bwd->setControl(&pressed_keys);

    // Концевые краны тормозной магистрали
    anglecock_bp_fwd->setKeySymbolOpen(KEY_F2);
    anglecock_bp_fwd->setKeyModifierOpen(MODIFIER_OnlyShift);
    anglecock_bp_fwd->setKeySymbolClose(KEY_F2);
    anglecock_bp_fwd->setKeyModifierClose(MODIFIER_OnlyControl);
    anglecock_bp_fwd->setControl(&pressed_keys);

    anglecock_bp_bwd->setKeySymbolOpen(KEY_F3);
    anglecock_bp_bwd->setKeyModifierOpen(MODIFIER_OnlyShift);
    anglecock_bp_bwd->setKeySymbolClose(KEY_F3);
    anglecock_bp_bwd->setKeyModifierClose(MODIFIER_OnlyControl);
    anglecock_bp_bwd->setControl(&pressed_keys);

    // Рукава тормозной магистрали
    hose_bp_fwd->setKeySymbolConnect(KEY_F1);
    hose_bp_fwd->setKeyModifierConnect(MODIFIER_OnlyShift);
    hose_bp_fwd->setKeySymbolDisconnect(KEY_F1);
    hose_bp_fwd->setKeyModifierDisconnect(MODIFIER_OnlyControl);
    hose_bp_fwd->setControl(&pressed_keys);

    hose_bp_bwd->setKeySymbolConnect(KEY_F4);
    hose_bp_bwd->setKeyModifierConnect(MODIFIER_OnlyShift);
    hose_bp_bwd->setKeySymbolDisconnect(KEY_F4);
    hose_bp_bwd->setKeyModifierDisconnect(MODIFIER_OnlyControl);
    hose_bp_bwd->setControl(&pressed_keys);


    // АЗВ "Компрессор"
    azv_motor_compressor.setKeySymbolOn(KEY_5);
    azv_motor_compressor.setKeyModifierOn(MODIFIER_OnlyShift);
    azv_motor_compressor.setKeySymbolOff(KEY_5);
    azv_motor_compressor.setKeyModifierOff(MODIFIER_OnlyControl);
    azv_motor_compressor.setControl(&pressed_keys);
    azv_motor_compressor.setInitState(true);

    for (auto cab_idx : {CAB1, CAB2})
    {
        // Устройство блокировки тормозов усл.№ 367
        brake_lock[cab_idx]->setControl(&pressed_keys_by_cabine[cab_idx]);

        // Электропневматический клапан автостопа
        epk[cab_idx]->setControl(&pressed_keys_by_cabine[cab_idx]);

        // Контроллер машиниста
        km[cab_idx]->setControl(&pressed_keys_by_cabine[cab_idx]);

        // АЗВ "Освещение кабины" (АВ 26)
        azv_cabine_light[cab_idx].setKeySymbolOn(KEY_K);
        azv_cabine_light[cab_idx].setKeyModifierOn(MODIFIER_OnlyShift);
        azv_cabine_light[cab_idx].setKeySymbolOff(KEY_K);
        azv_cabine_light[cab_idx].setKeyModifierOff(MODIFIER_OnlyControl);
        azv_cabine_light[cab_idx].setControl(&pressed_keys_by_cabine[cab_idx]);

        // АЗВ "Освещение пульта" (АВ 25)
        azv_panel_light[cab_idx].setKeySymbolOn(KEY_L);
        azv_panel_light[cab_idx].setKeyModifierOn(MODIFIER_OnlyShift);
        azv_panel_light[cab_idx].setKeySymbolOff(KEY_L);
        azv_panel_light[cab_idx].setKeyModifierOff(MODIFIER_OnlyControl);
        azv_panel_light[cab_idx].setControl(&pressed_keys_by_cabine[cab_idx]);

        // АЗВ "Управление общее" (АВ 2)
        azv_common_control[cab_idx].setKeySymbolOn(KEY_U);
        azv_common_control[cab_idx].setKeyModifierOn(MODIFIER_OnlyShift);
        azv_common_control[cab_idx].setKeySymbolOff(KEY_U);
        azv_common_control[cab_idx].setKeyModifierOff(MODIFIER_OnlyControl);
        azv_common_control[cab_idx].setControl(&pressed_keys_by_cabine[cab_idx]);

        // АЗВ "Управление тепловозом" (АВ 1)
        azv_upr_tepl[cab_idx].setKeySymbolOn(KEY_I);
        azv_upr_tepl[cab_idx].setKeyModifierOn(MODIFIER_OnlyShift);
        azv_upr_tepl[cab_idx].setKeySymbolOff(KEY_I);
        azv_upr_tepl[cab_idx].setKeyModifierOff(MODIFIER_OnlyControl);
        azv_upr_tepl[cab_idx].setControl(&pressed_keys_by_cabine[cab_idx]);

        // АЗВ "Топливный насос" (АВ 4)
        azv_fuel_pump[cab_idx].setKeySymbolOn(KEY_P);
        azv_fuel_pump[cab_idx].setKeyModifierOn(MODIFIER_OnlyShift);
        azv_fuel_pump[cab_idx].setKeySymbolOff(KEY_P);
        azv_fuel_pump[cab_idx].setKeyModifierOff(MODIFIER_OnlyControl);
        azv_fuel_pump[cab_idx].setControl(&pressed_keys_by_cabine[cab_idx]);

        // АЗВ "ЭДТ" (АВ 7)
        azv_edt_on[cab_idx].setKeySymbolOn(KEY_F);
        azv_edt_on[cab_idx].setKeyModifierOn(MODIFIER_OnlyShift);
        azv_edt_on[cab_idx].setKeySymbolOff(KEY_F);
        azv_edt_on[cab_idx].setKeyModifierOff(MODIFIER_OnlyControl);
        azv_edt_on[cab_idx].setControl(&pressed_keys_by_cabine[cab_idx]);

        // АЗВ "ЭПТ" (АВ 19)
        azv_ept_on[cab_idx].setKeySymbolOn(KEY_V);
        azv_ept_on[cab_idx].setKeyModifierOn(MODIFIER_OnlyShift);
        azv_ept_on[cab_idx].setKeySymbolOff(KEY_V);
        azv_ept_on[cab_idx].setKeyModifierOff(MODIFIER_OnlyControl);
        azv_ept_on[cab_idx].setControl(&pressed_keys_by_cabine[cab_idx]);

        // АЗВ "ЭПТ питание" (АВ 18)
        azv_ept_power[cab_idx].setKeySymbolOn(KEY_V);
        azv_ept_power[cab_idx].setKeyModifierOn(MODIFIER_OnlyAlt);
        azv_ept_power[cab_idx].setKeySymbolOff(KEY_V);
        azv_ept_power[cab_idx].setKeyModifierOff(MODIFIER_OnlyAlt);
        azv_ept_power[cab_idx].setControl(&pressed_keys_by_cabine[cab_idx]);

        // АЗВ "Прожектор тускло" (АВ 23)
        azv_spotlight_low[cab_idx].setKeySymbolOn(KEY_H);
        azv_spotlight_low[cab_idx].setKeyModifierOn(MODIFIER_OnlyShift);
        azv_spotlight_low[cab_idx].setKeySymbolOff(KEY_H);
        azv_spotlight_low[cab_idx].setKeyModifierOff(MODIFIER_OnlyControl);
        azv_spotlight_low[cab_idx].setControl(&pressed_keys_by_cabine[cab_idx]);

        // АЗВ "Прожектор ярко" (АВ 24)
        azv_spotlight_high[cab_idx].setKeySymbolOn(KEY_H);
        azv_spotlight_high[cab_idx].setKeyModifierOn(MODIFIER_OnlyAlt);
        azv_spotlight_high[cab_idx].setKeySymbolOff(KEY_H);
        azv_spotlight_high[cab_idx].setKeyModifierOff(MODIFIER_OnlyAlt);
        azv_spotlight_high[cab_idx].setControl(&pressed_keys_by_cabine[cab_idx]);

        // Тумблер "Ослабление поля I ступени руч./авт." (Тб 1)
        tumbler_field_weak1[cab_idx].setNumPositions(3);
        tumbler_field_weak1[cab_idx].setKeySymbolIncrease(KEY_1);
        tumbler_field_weak1[cab_idx].setKeyModifierIncrease(MODIFIER_OnlyShift);
        tumbler_field_weak1[cab_idx].setKeySymbolDecrease(KEY_1);
        tumbler_field_weak1[cab_idx].setKeyModifierDecrease(MODIFIER_OnlyControl);
        tumbler_field_weak1[cab_idx].setControl(&pressed_keys_by_cabine[cab_idx]);
        tumbler_field_weak1[cab_idx].setInitPosition(1);

        // Тумблер "Ослабление поля II ступени руч./авт." (Тб 2)
        tumbler_field_weak2[cab_idx].setNumPositions(3);
        tumbler_field_weak2[cab_idx].setKeySymbolIncrease(KEY_2);
        tumbler_field_weak2[cab_idx].setKeyModifierIncrease(MODIFIER_OnlyShift);
        tumbler_field_weak2[cab_idx].setKeySymbolDecrease(KEY_2);
        tumbler_field_weak2[cab_idx].setKeyModifierDecrease(MODIFIER_OnlyControl);
        tumbler_field_weak2[cab_idx].setControl(&pressed_keys_by_cabine[cab_idx]);
        tumbler_field_weak2[cab_idx].setInitPosition(1);

        // Тумблер "Управление жалюзи воды руч./авт." (Тб 8)
        tumbler_water_zaluzi[cab_idx].setNumPositions(3);
        tumbler_water_zaluzi[cab_idx].setKeySymbolIncrease(KEY_3);
        tumbler_water_zaluzi[cab_idx].setKeyModifierIncrease(MODIFIER_OnlyShift);
        tumbler_water_zaluzi[cab_idx].setKeySymbolDecrease(KEY_3);
        tumbler_water_zaluzi[cab_idx].setKeyModifierDecrease(MODIFIER_OnlyControl);
        tumbler_water_zaluzi[cab_idx].setControl(&pressed_keys_by_cabine[cab_idx]);
        tumbler_water_zaluzi[cab_idx].setInitPosition(1);

        // Тумблер "Управление жалюзи масла руч./авт." (Тб 9)
        tumbler_oil_zaluzi[cab_idx].setNumPositions(3);
        tumbler_oil_zaluzi[cab_idx].setKeySymbolIncrease(KEY_4);
        tumbler_oil_zaluzi[cab_idx].setKeyModifierIncrease(MODIFIER_OnlyShift);
        tumbler_oil_zaluzi[cab_idx].setKeySymbolDecrease(KEY_4);
        tumbler_oil_zaluzi[cab_idx].setKeyModifierDecrease(MODIFIER_OnlyControl);
        tumbler_oil_zaluzi[cab_idx].setControl(&pressed_keys_by_cabine[cab_idx]);
        tumbler_oil_zaluzi[cab_idx].setInitPosition(1);

        // Тумблер "Напряжение ЦУ. Напряжение ЭПТ" (Тб 15)
        tumbler_voltage[cab_idx].setKeySymbolOn(KEY_9);
        tumbler_voltage[cab_idx].setKeyModifierOn(MODIFIER_OnlyShift);
        tumbler_voltage[cab_idx].setKeySymbolOff(KEY_9);
        tumbler_voltage[cab_idx].setKeyModifierOff(MODIFIER_OnlyControl);
        tumbler_voltage[cab_idx].setControl(&pressed_keys_by_cabine[cab_idx]);

        // Тумблер "Аварийная остановка дизеля" (Тб 6)
        tumbler_disel_stop[cab_idx].setKeySymbolOn(KEY_O);
        tumbler_disel_stop[cab_idx].setKeyModifierOn(MODIFIER_OnlyShift);
        tumbler_disel_stop[cab_idx].setKeySymbolOff(KEY_O);
        tumbler_disel_stop[cab_idx].setKeyModifierOff(MODIFIER_OnlyControl);
        tumbler_disel_stop[cab_idx].setControl(&pressed_keys_by_cabine[cab_idx]);
        tumbler_disel_stop[cab_idx].setInitState(true);

        // Тумблер "Буферный фонарь левый" (Тб 35)
        tumbler_bufferliht_L[cab_idx].setNumPositions(3);
        tumbler_bufferliht_L[cab_idx].setKeySymbolIncrease(KEY_G);
        tumbler_bufferliht_L[cab_idx].setKeyModifierIncrease(MODIFIER_OnlyShift);
        tumbler_bufferliht_L[cab_idx].setKeySymbolDecrease(KEY_G);
        tumbler_bufferliht_L[cab_idx].setKeyModifierDecrease(MODIFIER_OnlyControl);
        tumbler_bufferliht_L[cab_idx].setControl(&pressed_keys_by_cabine[cab_idx]);
        tumbler_bufferliht_L[cab_idx].setInitPosition(1);

        // Тумблер "Буферный фонарь левый" (Тб 35)
        tumbler_bufferliht_R[cab_idx].setNumPositions(3);
        tumbler_bufferliht_R[cab_idx].setKeySymbolIncrease(KEY_J);
        tumbler_bufferliht_R[cab_idx].setKeyModifierIncrease(MODIFIER_OnlyShift);
        tumbler_bufferliht_R[cab_idx].setKeySymbolDecrease(KEY_J);
        tumbler_bufferliht_R[cab_idx].setKeyModifierDecrease(MODIFIER_OnlyControl);
        tumbler_bufferliht_R[cab_idx].setControl(&pressed_keys_by_cabine[cab_idx]);
        tumbler_bufferliht_R[cab_idx].setInitPosition(1);

        // Тумблер "Номерные знаки" (Тб 37)
        tumbler_number_light[cab_idx].setKeySymbolOn(KEY_Backslash);
        tumbler_number_light[cab_idx].setKeyModifierOn(MODIFIER_OnlyShift);
        tumbler_number_light[cab_idx].setKeySymbolOff(KEY_Backslash);
        tumbler_number_light[cab_idx].setKeyModifierOff(MODIFIER_OnlyControl);
        tumbler_number_light[cab_idx].setControl(&pressed_keys_by_cabine[cab_idx]);

        // Кнопка "Пуск дизеля" (Кн 3)
        button_disel_start[cab_idx].setKeySymbolOn(KEY_P);
        button_disel_start[cab_idx].setKeyModifierOn(MODIFIER_OnlyAlt);
        button_disel_start[cab_idx].setKeySymbolOff(KEY_Undefined);
        button_disel_start[cab_idx].setKeyModifierOff(KEY_Undefined);
        button_disel_start[cab_idx].setControl(&pressed_keys_by_cabine[cab_idx]);

        // Кнопка "Отпуск тормозов" (Кн 17)
        button_brake_release[cab_idx].setKeySymbolOn(KEY_Slash);
        button_brake_release[cab_idx].setKeyModifierOn(ANY_MODIFIERS);
        button_brake_release[cab_idx].setKeySymbolOff(KEY_Undefined);
        button_brake_release[cab_idx].setKeyModifierOff(KEY_Undefined);
        button_brake_release[cab_idx].setControl(&pressed_keys_by_cabine[cab_idx]);

        // Кнопка "Свисток" (Кн 7)
        button_svistok[cab_idx].setKeySymbolOn(KEY_Space);
        button_svistok[cab_idx].setKeyModifierOn(ANY_MODIFIERS);
        button_svistok[cab_idx].setKeySymbolOff(KEY_Undefined);
        button_svistok[cab_idx].setKeyModifierOff(KEY_Undefined);
        button_svistok[cab_idx].setControl(&pressed_keys_by_cabine[cab_idx]);

        // Кнопка "Тифон" (Кн 6)
        button_tifon[cab_idx].setKeySymbolOn(KEY_B);
        button_tifon[cab_idx].setKeyModifierOn(ANY_MODIFIERS);
        button_tifon[cab_idx].setKeySymbolOff(KEY_Undefined);
        button_tifon[cab_idx].setKeyModifierOff(KEY_Undefined);
        button_tifon[cab_idx].setControl(&pressed_keys_by_cabine[cab_idx]);


        // Триггеры рукояток бдительности
        rb[cab_idx][RBS].setKeySymbolOn(KEY_M);
        rb[cab_idx][RBS].setKeyModifierOn(ANY_MODIFIERS);
        rb[cab_idx][RBS].setKeySymbolOff(KEY_Undefined);
        rb[cab_idx][RBS].setKeyModifierOff(KEY_Undefined);
        rb[cab_idx][RBS].setControl(&pressed_keys_by_cabine[cab_idx]);

        rb[cab_idx][RB].setKeySymbolOn(KEY_Z);
        rb[cab_idx][RB].setKeyModifierOn(ANY_MODIFIERS);
        rb[cab_idx][RB].setKeySymbolOff(KEY_Undefined);
        rb[cab_idx][RB].setKeyModifierOff(KEY_Undefined);
        rb[cab_idx][RB].setControl(&pressed_keys_by_cabine[cab_idx]);

        rb[cab_idx][RBP].setKeySymbolOn(KEY_Tilde);
        rb[cab_idx][RBP].setKeyModifierOn(ANY_MODIFIERS);
        rb[cab_idx][RBP].setKeySymbolOff(KEY_Undefined);
        rb[cab_idx][RBP].setKeyModifierOff(KEY_Undefined);
        rb[cab_idx][RBP].setControl(&pressed_keys_by_cabine[cab_idx]);

        // Переключатель тормозного усилия
        brake_force_switch[cab_idx].setNumPositions(12);
        brake_force_switch[cab_idx].setKeySymbolIncrease(KEY_Period);
        brake_force_switch[cab_idx].setKeyModifierIncrease(ANY_MODIFIERS);
        brake_force_switch[cab_idx].setKeySymbolDecrease(KEY_Comma);
        brake_force_switch[cab_idx].setKeyModifierDecrease(ANY_MODIFIERS);
        brake_force_switch[cab_idx].setControl(&pressed_keys_by_cabine[cab_idx]);
        brake_force_switch[cab_idx].setInitPosition(0);
    }
}
