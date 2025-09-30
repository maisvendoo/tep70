#include    "tep70bs.h"

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------
void TEP70BS::initControl(const QString &modules_dir, const QString &custom_cfg_dir)
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


    for (auto cab_idx : {CAB1, CAB2})
    {
        // Устройство блокировки тормозов усл.№ 367
        brake_lock[cab_idx]->setControl(&pressed_keys_by_cabine[cab_idx]);

        // АЗВ "Управление общее" (АВ2)
        azv_common_control[cab_idx].setKeySymbolOn(KEY_Y);
        azv_common_control[cab_idx].setKeyModifierOn(MODIFIER_OnlyShift);
        azv_common_control[cab_idx].setKeySymbolOff(KEY_Y);
        azv_common_control[cab_idx].setKeyModifierOff(MODIFIER_OnlyControl);
        azv_common_control[cab_idx].setControl(&pressed_keys_by_cabine[cab_idx]);

        // АЗВ "Управление тепловозом" (АВ1)
        azv_upr_tepl[cab_idx].setKeySymbolOn(KEY_U);
        azv_upr_tepl[cab_idx].setKeyModifierOn(MODIFIER_OnlyShift);
        azv_upr_tepl[cab_idx].setKeySymbolOff(KEY_U);
        azv_upr_tepl[cab_idx].setKeyModifierOff(MODIFIER_OnlyControl);
        azv_upr_tepl[cab_idx].setControl(&pressed_keys_by_cabine[cab_idx]);

        // АЗВ "Топливный насос" (АВ4)
        azv_fuel_pump[cab_idx].setKeySymbolOn(KEY_P);
        azv_fuel_pump[cab_idx].setKeyModifierOn(MODIFIER_OnlyShift);
        azv_fuel_pump[cab_idx].setKeySymbolOff(KEY_P);
        azv_fuel_pump[cab_idx].setKeyModifierOff(MODIFIER_OnlyControl);
        azv_fuel_pump[cab_idx].setControl(&pressed_keys_by_cabine[cab_idx]);

        // АЗВ "ЭДТ"
        azv_edt_on[cab_idx].setKeySymbolOn(KEY_F);
        azv_edt_on[cab_idx].setKeyModifierOn(MODIFIER_OnlyShift);
        azv_edt_on[cab_idx].setKeySymbolOff(KEY_F);
        azv_edt_on[cab_idx].setKeyModifierOff(MODIFIER_OnlyControl);
        azv_edt_on[cab_idx].setControl(&pressed_keys_by_cabine[cab_idx]);

        // АЗВ "Тормоз питание"
        azv_edt_power[cab_idx].setKeySymbolOn(KEY_R);
        azv_edt_power[cab_idx].setKeyModifierOn(MODIFIER_OnlyShift);
        azv_edt_power[cab_idx].setKeySymbolOff(KEY_R);
        azv_edt_power[cab_idx].setKeyModifierOff(MODIFIER_OnlyControl);
        azv_edt_power[cab_idx].setControl(&pressed_keys_by_cabine[cab_idx]);

        // АЗВ "ЭПТ"
        azv_ept_on[cab_idx].setKeySymbolOn(KEY_V);
        azv_ept_on[cab_idx].setKeyModifierOn(MODIFIER_OnlyShift);
        azv_ept_on[cab_idx].setKeySymbolOff(KEY_V);
        azv_ept_on[cab_idx].setKeyModifierOff(MODIFIER_OnlyControl);
        azv_ept_on[cab_idx].setControl(&pressed_keys_by_cabine[cab_idx]);

        // АЗВ "Компрессор"
        azv_motor_compressor[cab_idx].setKeySymbolOn(KEY_5);
        azv_motor_compressor[cab_idx].setKeyModifierOn(MODIFIER_OnlyShift);
        azv_motor_compressor[cab_idx].setKeySymbolOff(KEY_5);
        azv_motor_compressor[cab_idx].setKeyModifierOff(MODIFIER_OnlyControl);
        azv_motor_compressor[cab_idx].setControl(&pressed_keys_by_cabine[cab_idx]);
        azv_motor_compressor[cab_idx].setInitState(true);

        // Тумблер "Напряжение ЦУ. Напряжение ЭПТ"
        tumbler_voltage[cab_idx].setKeySymbolOn(KEY_9);
        tumbler_voltage[cab_idx].setKeyModifierOn(MODIFIER_OnlyShift);
        tumbler_voltage[cab_idx].setKeySymbolOff(KEY_9);
        tumbler_voltage[cab_idx].setKeyModifierOff(MODIFIER_OnlyControl);
        tumbler_voltage[cab_idx].setControl(&pressed_keys_by_cabine[cab_idx]);

        // Тумблер "Аварийная остановка дизеля"
        tumbler_disel_stop[cab_idx].setKeySymbolOn(KEY_O);
        tumbler_disel_stop[cab_idx].setKeyModifierOn(MODIFIER_OnlyShift);
        tumbler_disel_stop[cab_idx].setKeySymbolOff(KEY_O);
        tumbler_disel_stop[cab_idx].setKeyModifierOff(MODIFIER_OnlyControl);
        tumbler_disel_stop[cab_idx].setControl(&pressed_keys_by_cabine[cab_idx]);

        // Тумблер "Ослабление поля I ступени руч./авт."
        tumbler_field_weak1[cab_idx].setNumPositions(3);
        tumbler_field_weak1[cab_idx].setKeySymbolIncrease(KEY_1);
        tumbler_field_weak1[cab_idx].setKeyModifierIncrease(MODIFIER_OnlyShift);
        tumbler_field_weak1[cab_idx].setKeySymbolDecrease(KEY_1);
        tumbler_field_weak1[cab_idx].setKeyModifierDecrease(MODIFIER_OnlyControl);
        tumbler_field_weak1[cab_idx].setControl(&pressed_keys_by_cabine[cab_idx]);
        tumbler_field_weak1[cab_idx].setInitPosition(1);

        // Тумблер "Ослабление поля II ступени руч./авт."
        tumbler_field_weak2[cab_idx].setNumPositions(3);
        tumbler_field_weak2[cab_idx].setKeySymbolIncrease(KEY_2);
        tumbler_field_weak2[cab_idx].setKeyModifierIncrease(MODIFIER_OnlyShift);
        tumbler_field_weak2[cab_idx].setKeySymbolDecrease(KEY_2);
        tumbler_field_weak2[cab_idx].setKeyModifierDecrease(MODIFIER_OnlyControl);
        tumbler_field_weak2[cab_idx].setControl(&pressed_keys_by_cabine[cab_idx]);
        tumbler_field_weak2[cab_idx].setInitPosition(1);

        // Тумблер "Управление жалюзи воды руч./авт."
        tumbler_water_zaluzi[cab_idx].setNumPositions(3);
        tumbler_water_zaluzi[cab_idx].setKeySymbolIncrease(KEY_3);
        tumbler_water_zaluzi[cab_idx].setKeyModifierIncrease(MODIFIER_OnlyShift);
        tumbler_water_zaluzi[cab_idx].setKeySymbolDecrease(KEY_3);
        tumbler_water_zaluzi[cab_idx].setKeyModifierDecrease(MODIFIER_OnlyControl);
        tumbler_water_zaluzi[cab_idx].setControl(&pressed_keys_by_cabine[cab_idx]);
        tumbler_water_zaluzi[cab_idx].setInitPosition(1);

        // Тумблер "Управление жалюзи масла руч./авт."
        tumbler_oil_zaluzi[cab_idx].setNumPositions(3);
        tumbler_oil_zaluzi[cab_idx].setKeySymbolIncrease(KEY_4);
        tumbler_oil_zaluzi[cab_idx].setKeyModifierIncrease(MODIFIER_OnlyShift);
        tumbler_oil_zaluzi[cab_idx].setKeySymbolDecrease(KEY_4);
        tumbler_oil_zaluzi[cab_idx].setKeyModifierDecrease(MODIFIER_OnlyControl);
        tumbler_oil_zaluzi[cab_idx].setControl(&pressed_keys_by_cabine[cab_idx]);
        tumbler_oil_zaluzi[cab_idx].setInitPosition(1);

        // Переключатель тормозного усилия
        brake_force_switch[cab_idx].setNumPositions(12);
        brake_force_switch[cab_idx].setKeySymbolIncrease(KEY_Period);
        brake_force_switch[cab_idx].setKeyModifierIncrease(ANY_MODIFIERS);
        brake_force_switch[cab_idx].setKeySymbolDecrease(KEY_Comma);
        brake_force_switch[cab_idx].setKeyModifierDecrease(ANY_MODIFIERS);
        brake_force_switch[cab_idx].setControl(&pressed_keys_by_cabine[cab_idx]);
        brake_force_switch[cab_idx].setInitPosition(0);

        // Кнопка "Пуск дизеля"
        button_disel_start[cab_idx].setKeySymbolOn(KEY_P);
        button_disel_start[cab_idx].setKeyModifierOn(MODIFIER_OnlyAlt);
        button_disel_start[cab_idx].setKeySymbolOff(KEY_Undefined);
        button_disel_start[cab_idx].setKeyModifierOff(KEY_Undefined);
        button_disel_start[cab_idx].setControl(&pressed_keys_by_cabine[cab_idx]);

        // Кнопка "Отпуск тормозов"
        button_brake_release[cab_idx].setKeySymbolOn(KEY_Slash);
        button_brake_release[cab_idx].setKeyModifierOn(ANY_MODIFIERS);
        button_brake_release[cab_idx].setKeySymbolOff(KEY_Undefined);
        button_brake_release[cab_idx].setKeyModifierOff(KEY_Undefined);
        button_brake_release[cab_idx].setControl(&pressed_keys_by_cabine[cab_idx]);

        // Кнопка "Свисток"
        button_svistok[cab_idx].setKeySymbolOn(KEY_Space);
        button_svistok[cab_idx].setKeyModifierOn(ANY_MODIFIERS);
        button_svistok[cab_idx].setKeySymbolOff(KEY_Undefined);
        button_svistok[cab_idx].setKeyModifierOff(KEY_Undefined);
        button_svistok[cab_idx].setControl(&pressed_keys_by_cabine[cab_idx]);

        // Кнопка "Тифон"
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

        rb[cab_idx][RB1].setKeySymbolOn(KEY_Z);
        rb[cab_idx][RB1].setKeyModifierOn(ANY_MODIFIERS);
        rb[cab_idx][RB1].setKeySymbolOff(KEY_Undefined);
        rb[cab_idx][RB1].setKeyModifierOff(KEY_Undefined);
        rb[cab_idx][RB1].setControl(&pressed_keys_by_cabine[cab_idx]);

        rb[cab_idx][RBP].setKeySymbolOn(KEY_Tilde);
        rb[cab_idx][RBP].setKeyModifierOn(ANY_MODIFIERS);
        rb[cab_idx][RBP].setKeySymbolOff(KEY_Undefined);
        rb[cab_idx][RBP].setKeyModifierOff(KEY_Undefined);
        rb[cab_idx][RBP].setControl(&pressed_keys_by_cabine[cab_idx]);

        // Ключ ЭПК
        key_epk[cab_idx].setKeySymbolOn(KEY_N);
        key_epk[cab_idx].setKeyModifierOn(MODIFIER_OnlyShift);
        key_epk[cab_idx].setKeySymbolOff(KEY_N);
        key_epk[cab_idx].setKeyModifierOff(MODIFIER_OnlyControl);
        key_epk[cab_idx].setControl(&pressed_keys_by_cabine[cab_idx]);

        // Переключатель реверсора
        tumbler_revers[cab_idx].setNumPositions(3);
        tumbler_revers[cab_idx].setKeySymbolIncrease(KEY_W);
        tumbler_revers[cab_idx].setKeyModifierIncrease(ANY_MODIFIERS);
        tumbler_revers[cab_idx].setKeySymbolDecrease(KEY_S);
        tumbler_revers[cab_idx].setKeyModifierDecrease(ANY_MODIFIERS);
        tumbler_revers[cab_idx].setControl(&pressed_keys_by_cabine[cab_idx]);
        tumbler_revers[cab_idx].setInitPosition(1);
    }
}
