#ifndef     TEP70_CONTROLS_H
#define     TEP70_CONTROLS_H

enum
{
    /// АЗВ
    CTRL_AZV_CABINE_LIGHT      = 100,
    CTRL_AZV_PANEL_LIGHT       = 101,
    CTRL_AZV_COMMON_CONTROL    = 102,
    CTRL_AZV_UPR_TEPL          = 103,
    CTRL_AZV_FUEL_PUMP         = 104,
    CTRL_AZV_EDT_ON            = 105,
    CTRL_AZV_EPT_ON            = 106,
    CTRL_AZV_EPT_POWER         = 107,
    CTRL_AZV_SPOTLIGHT_LOW     = 108,
    CTRL_AZV_SPOTLIGHT_HIGH    = 109,

    /// Тумблеры (3-поз)
    CTRL_TUMBLER_FIELD_WEAK1   = 110,
    CTRL_TUMBLER_FIELD_WEAK2   = 111,
    CTRL_TUMBLER_WATER_ZALUZI  = 112,
    CTRL_TUMBLER_OIL_ZALUZI    = 113,

    /// Тумблеры (Toggle)
    CTRL_TUMBLER_VOLTMETER         = 114,
    CTRL_TUMBLER_DISEL_STOP        = 115,
    CTRL_TUMBLER_BUFFERLIGHT_L     = 116,
    CTRL_TUMBLER_BUFFERLIGHT_R     = 117,
    CTRL_TUMBLER_NUMBER_LIGHT      = 118,
    CTRL_TUMBLER_PANEL_POM_LIGHT   = 119,
    CTRL_TUMBLER_RESERVE_LIGHT     = 120,
    CTRL_TUMBLER_CAB_HEATER        = 121,
    CTRL_TUMBLER_CAB_FAN           = 122,

    /// Кнопки
    CTRL_BUTTON_DISEL_START    = 123,
    CTRL_BUTTON_BRAKE_RELEASE  = 124,
    CTRL_BUTTON_SVISTOK        = 125,
    CTRL_BUTTON_TIFON          = 126,

    /// РБ
    CTRL_RBS  = 127,
    CTRL_RB   = 128,
    CTRL_RBP  = 129,

    /// Переключатели
    CTRL_BRAKE_FORCE_SWITCH    = 130,

    /// КМ, ЭПК, УБТ
    CTRL_REVERS_INSERTION      = 140,
    CTRL_EPK_INSERTION         = 141,
    CTRL_LOCK_367_INSERTION    = 142,
    CTRL_KEY_EPK               = 143,
    CTRL_REVERS_POSITION       = 144,
    CTRL_KM_MAIN_POSITION      = 145,

    /// Тормозные краны
    CTRL_KRM_395               = 150,
    CTRL_KVT_254               = 151,
    CTRL_COMBINE_KRAN          = 152,

    /// Shared (машинное отделение)
    CTRL_AZV_MOTOR_COMPRESSOR  = 160,

    /// АЗВ ВВК (общие)
    CTRL_AZV_BUFFER_LIGHTS     = 242,
    CTRL_AZV_CONTROL_CIRCUIT   = 243,
    CTRL_AZV_DISEL_LIGHT       = 244,
    CTRL_AZV_EPK_POWER_MINUS   = 245,
    CTRL_AZV_EPK_POWER_PLUS    = 246,
    CTRL_AZV_FIRE_ALARM        = 247,
    CTRL_AZV_FUEL_PUMP_VVK     = 248,
    CTRL_AZV_HEATER            = 249,
    CTRL_AZV_SOCKETS_POWER     = 250,
    CTRL_AZV_SOCKETS_POWER_2   = 251,
    CTRL_AZV_TROLLEYS_LIGHT    = 252,
    CTRL_AZV_VVK_LIGHT         = 253,
    CTRL_AZV_WATCH_DISEL_LIGHT = 254,

    /// Тумблеры пульта
    CTRL_TUMBLER_GAUGES_POWER = 294,
    CTRL_TUMBLER_RADIO_KV     = 295,
    CTRL_TUMBLER_RADIO_UKV    = 296,
    CTRL_TUMBLER_ALSN_FREQ    = 297,

    /// Органы панели ВВК
    CTRL_VVK_EDT_TUMBLER       = 255,
    CTRL_VVK_SHUNTING_TUMBLER  = 256,
    CTRL_VVK_MANUAL_FUEL_PUMP  = 257,
    CTRL_VVK_RESERVED_FUEL_PUMP = 258,
    CTRL_VVK_AUTOSTOP_TUMBLER  = 259,
    CTRL_VVK_ISOLATION_CHECK   = 260,
    CTRL_VVK_FOUND_GROUND_SWITCH = 261,
    CTRL_VVK_GROUND_RELAY_BUTTON = 262,
    CTRL_VVK_KEEP_FUEL_PUMP_BUTTON = 263,
    CTRL_VVK_V_MINUS_BUTTON        = 264,
    CTRL_VVK_V_PLUS_BUTTON         = 265,

    /// Тумблеры отключателей ТЭД (ОМ)
    CTRL_TUMBLER_OM1              = 266,
    CTRL_TUMBLER_OM2              = 267,
    CTRL_TUMBLER_OM3              = 268,
    CTRL_TUMBLER_OM4              = 269,
    CTRL_TUMBLER_OM5              = 270,
    CTRL_TUMBLER_OM6              = 271,

    /// Переключатели ВВК
    CTRL_VVK_PA1                  = 272,
    CTRL_VVK_PA2                  = 273,
    CTRL_VVK_KB1                  = 274,
    CTRL_VVK_KB2                  = 275,
    CTRL_VVK_RC1                  = 276,
    CTRL_VVK_RC2                  = 277,

    /// Тумблер числа тепловозов
    CTRL_TUMBLER_LOCO_COUNT       = 278,

    /// Рубильник аккумуляторной батареи
    CTRL_ACCUM_SWITCH             = 279,

    /// Автоведение
    CTRL_AUTOPILOT             = 170,
    CTRL_SHUNTING_MODE         = 171,

    /// Команды (кнопки-действия)
    CTRL_AUTOSTART_PROGRAM     = 180,
    CTRL_AUTOSTOP_PROGRAM      = 181,

    /// Coupling / hose / anglecock (shared index — без ObjectName)
    CTRL_OPER_ROD_FWD          = 200,
    CTRL_OPER_ROD_BWD          = 201,
    CTRL_OPER_ROD_FIX_FWD      = 214,
    CTRL_OPER_ROD_FIX_BWD      = 215,
    CTRL_ANGLECOCK_BC_FWD      = 202,
    CTRL_ANGLECOCK_BC_BWD      = 203,
    CTRL_HOSE_BC_FWD           = 204,
    CTRL_HOSE_BC_BWD           = 205,
    CTRL_ANGLECOCK_FL_FWD      = 206,
    CTRL_ANGLECOCK_FL_BWD      = 207,
    CTRL_HOSE_FL_FWD           = 208,
    CTRL_HOSE_FL_BWD           = 209,
    CTRL_ANGLECOCK_BP_FWD      = 210,
    CTRL_ANGLECOCK_BP_BWD      = 211,
    CTRL_HOSE_BP_FWD           = 212,
    CTRL_HOSE_BP_BWD           = 213,

    /// Переключение кабин
    CTRL_CABINE_1              = 230,
    CTRL_CABINE_2              = 231,

    /// Кнопки пульта помощника
    CTRL_BUTTON_SVISTOK_POM    = 190,
    CTRL_BUTTON_TIFON_POM      = 191,

    // Прочее
    CTRL_SAND_BUTTON           = 220,

    /// Тумблер ПСС (декоративный, не влияет на схему)
    CTRL_TUMBLER_PSS           = 232,

    CTRL_VVK_BLINDS_DOOR = 240,
    CTRL_VVK_DOOR = 241,
    CTRL_CABINE1_DOOR = 280,
    CTRL_CABINE2_DOOR = 281,

    /// Тумблер отопления кабины
    CTRL_TUMBLER_HEATER       = 282,

    /// АЗВ вентиляции кабины
    CTRL_AZV_VENT             = 283,

    /// Регулятор подсветки приборов
    CTRL_GAUGES_LIGHT         = 284,

    /// Регулятор подсветки расписания
    CTRL_TIMETABLE_LIGHT      = 285,

    /// Тумблеры освещения кабины (собственные)
    CTRL_TUMBLER_CABINE_LIGHT = 286,
};

#endif
