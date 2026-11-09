//------------------------------------------------------------------------------
//
//      Магистральный пассажирский тепловоз ТЭП70.
//      Дополнение для Russian Railway Simulator (RRS)
//
//      (c) RRS development team:
//          Дмитрий Притыкин (maisvendoo),
//          Роман Бирюков (РомычРЖДУЗ)
//
//      Дата: 12/05/2019
//
//------------------------------------------------------------------------------
#ifndef     TEP70_H
#define     TEP70_H

#include    <vehicle-api.h>
#include    <tep70-signals.h>

#include    <fuel-tank.h>
#include    <electric-fuel-pump.h>
#include    <disel.h>
#include    <electric-oil-pump.h>
#include    <starter-generator.h>
#include    <voltage-regulator.h>
#include    <tep70-motor-compressor.h>
#include    <field-generator.h>
#include    <trac-generator.h>
#include    <field-regulator.h>
#include    <trac-motor.h>
#include    <sl2m.h>
#include    <km-2202.h>
#include    <reversor.h>
#include    <brake-switcher.h>
#include    <alsn-ukbm.h>
#include    <tep70-autopilot-types.h>

#include    <hydrostatic-fan-drive.h>
#include    <cooler-fan.h>
#include    <hysteresis-relay.h>
#include    <pneumo-electro-valve.h>
#include    <logic-relay.h>
#include    <handbrake-actor.h>
#include    <electro-pneumatic-valve.h>
#include    <blinds-actor.h>
#include    <edb-control-unit.h>

/*!
 * \class
 * \brief Основной класс, описывающий весь тепловоз
 */
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
class TEP70 : public Vehicle
{
public:

    /// Конструктор
    TEP70();

    /// Деструктор
    ~TEP70();

    /// Инициализация тормозных приборов
    void initBrakeDevices(double p0, double pBP, double pFL) override;

private:

    /// Имя модуля сцепного устройства
    QString coupling_module_name = "sa3";
    /// Имя конфига сцепного устройства
    QString coupling_config_name = "sa3";
    /// Имя модуля поездного крана
    QString brake_crane_module_name = "krm395";
    /// Имя конфига поездного крана
    QString brake_crane_config_name = "krm395";
    /// Имя модуля локомотивного крана
    QString loco_crane_module_name = "kvt254";
    /// Имя конфига локомотивного крана
    QString loco_crane_config_name = "kvt254";
    /// Имя модуля воздухораспределителя
    QString airdist_module_name = "vr292";
    /// Имя конфига воздухорапределителя
    QString airdist_config_name = "vr292";
    /// Имя модуля электровоздухораспределителя
    QString electro_airdist_module_name = "evr305";
    /// Имя конфига электровоздухорапределителя
    QString electro_airdist_config_name = "evr305";

    /// Имя модуля автоведения
    QString autopilot_module_name = "tep70-autopilot";
    /// Имя конфига модуля автоведения
    QString autopilot_config_name = "tep70-autopilot";
    /// Каталог поиска кастомных модулей
    QString custom_modules_dir = "tep70";

    /// Сцепка спереди
    Coupling* coupling_fwd = nullptr;
    /// Сцепка сзади
    Coupling* coupling_bwd = nullptr;

    /// Расцепной рычаг спереди
    OperatingRod* oper_rod_fwd = nullptr;
    /// Расцепной рычаг сзади
    OperatingRod* oper_rod_bwd = nullptr;

    /// АЗВ "Компрессор" (АВ 9)
    TriggerControl  azv_motor_compressor;

    /// АЗВ "Освещение буферных огней"
    TriggerControl  azv_buffer_lights;

    /// АЗВ "Цепи управления"
    TriggerControl  azv_control_circuit;

    /// АЗВ "Освещение дизеля"
    TriggerControl  azv_disel_light;

    /// АЗВ "Питание ЭПК (-)"
    TriggerControl  azv_epk_power_minus;

    /// АЗВ "Питание ЭПК (+)"
    TriggerControl  azv_epk_power_plus;

    /// АЗВ "Пожарная сигнализация"
    TriggerControl  azv_fire_alarm;

    /// АЗВ "Топливный насос" (ВВК)
    TriggerControl  azv_fuel_pump_vvk;

    /// АЗВ "Отопление"
    TriggerControl  azv_heater;

    /// АЗВ "Розетки (1)"
    TriggerControl  azv_sockets_power;

    /// АЗВ "Розетки (2)"
    TriggerControl  azv_gauges_power;

    /// АЗВ "Освещение тележек"
    TriggerControl  azv_trolleys_light;

    /// АЗВ "Освещение ВВК"
    TriggerControl  azv_vvk_light;

    /// АЗВ "Освещение осмотра дизеля"
    TriggerControl  azv_watch_disel_light;

    /// Органы управления панели ВВК
    /// Тумблер ЭДТ (электродинамический тормоз)
    TriggerControl  edt_tumbler;

    /// Тумблер шунтировки
    TriggerControl  shunting_tumbler;

    /// Тумблер ручной подкачки топлива
    TriggerControl  manual_fuel_pumping;

    /// Тумблер резервного топливного насоса
    TriggerControl  reserved_fuel_pump;

    /// Тумблер автостопа
    TriggerControl  autostop_tumbler;

    /// Тумблер контроля сопротивления изоляции
    TriggerControl  isolation_check;

    /// Переключатель обнаружения заземления (20 позиций)
    SwitcherControl found_ground_switch;

    /// Кнопка возврата реле земли
    TriggerControl  ground_relay_button;

    /// Кнопка удержания питания топливного насоса
    TriggerControl  keep_fuel_pump_button;

    /// Кнопка регулировки напряжения минус
    TriggerControl  v_minus_button;

    /// Кнопка регулировки напряжения плюс
    TriggerControl  v_plus_button;

    enum
    {
        /// Число кабин
        CABS_NUM = 2,
        /// Индекс 1 кабины
        CAB1 = 0,
        /// Индекс 2 кабины
        CAB2 = 1
    };

    /// АЗВ "Освещение кабины" (АВ 26)
    TriggerControl  azv_cabine_light[CABS_NUM];

    /// АЗВ "Освещение пульта" (АВ 25)
    TriggerControl  azv_panel_light[CABS_NUM];

    /// АЗВ "Управление общее" (АВ 2)
    TriggerControl  azv_common_control[CABS_NUM];

    /// АЗВ "Управление тепловозом" (АВ 1)
    TriggerControl  azv_upr_tepl[CABS_NUM];

    /// АЗВ "Топливный насос" (АВ 4)
    TriggerControl  azv_fuel_pump[CABS_NUM];

    /// АЗВ "ЭДТ" (АВ 7)
    TriggerControl  azv_edt_on[CABS_NUM];

    /// АЗВ "ЭПТ" (АВ 19)
    TriggerControl  azv_ept_on[CABS_NUM];

    /// АЗВ "ЭПТ питание" (АВ 18)
    TriggerControl  azv_ept_power[CABS_NUM];

    /// АЗВ "Прожектор тускло" (АВ 23)
    TriggerControl  azv_spotlight_low[CABS_NUM];

    /// АЗВ "Прожектор ярко" (АВ 24)
    TriggerControl  azv_spotlight_high[CABS_NUM];

    /// Тумблер "Ослабление поля I ступени руч./авт." (Тб 1)
    SwitcherControl tumbler_field_weak1[CABS_NUM];

    /// Тумблер "Ослабление поля II ступени руч./авт." (Тб 2)
    SwitcherControl tumbler_field_weak2[CABS_NUM];

    /// Тумблер "Управление жалюзи воды руч./авт." (Тб 8)
    SwitcherControl tumbler_water_zaluzi[CABS_NUM];

    /// Тумблер "Управление жалюзи масла руч./авт." (Тб 9)
    SwitcherControl tumbler_oil_zaluzi[CABS_NUM];

    /// Тумблер "Напряжение ЦУ / Напряжение ЭПТ" (Тб 15)
    TriggerControl  tumbler_voltage[CABS_NUM];

    /// Тумблер "Аварийная остановка дизеля" (Тб 6)
    TriggerControl  tumbler_disel_stop[CABS_NUM];

    /// Тумблер "Буферный фонарь левый" (Тб 35)
    SwitcherControl tumbler_bufferliht_L[CABS_NUM];

    /// Тумблер "Буферный фонарь правый" (Тб 36)
    SwitcherControl tumbler_bufferliht_R[CABS_NUM];

    /// Тумблер "Номерные знаки" (Тб 37)
    TriggerControl tumbler_number_light[CABS_NUM];

    /// Тумблер "Подсветка пульта помощника"
    TriggerControl tumbler_panel_pom_light[CABS_NUM];

    /// Тумблер "Резервный свет"
    TriggerControl tumbler_reserve_light[CABS_NUM];

    /// Тумблер "Калорифер кабины"
    TriggerControl tumbler_cab_heater[CABS_NUM];

    /// Тумблер "Вентилятор кабины"
    TriggerControl tumbler_cab_fan[CABS_NUM];

    /// Кнопка "Пуск дизеля" (Кн 3)
    TriggerControl  button_disel_start[CABS_NUM];

    /// Кнопка "Отпуск тормозов" (Кн 17)
    TriggerControl  button_brake_release[CABS_NUM];

    /// Кнопка "Свисток" (Кн 7)
    TriggerControl  button_svistok[CABS_NUM];

    /// Кнопка "Тифон" (Кн 6)
    TriggerControl  button_tifon[CABS_NUM];

    /// Кнопка "Свисток" (пульт помощника)
    TriggerControl  button_svistok_pom[CABS_NUM];

    /// Кнопка "Тифон" (пульт помощника)
    TriggerControl  button_tifon_pom[CABS_NUM];

    /// Кнопка "Пескоподача"
    TriggerControl  button_sand[CABS_NUM];

    /// Тумблер ПСС (декоративный, не влияет на схему)
    TriggerControl  tumbler_pss[CABS_NUM];

    /// Тумблеры пульта
    TriggerControl  tumbler_gauges_power[CABS_NUM];
    TriggerControl  tumbler_radio_kv[CABS_NUM];
    TriggerControl  tumbler_radio_ukv[CABS_NUM];
    TriggerControl  tumbler_alsn_freq[CABS_NUM];

    /// Кнопки пульта
    TriggerControl  button_maneuver[CABS_NUM];
    TriggerControl  button_svistok_maneuver[CABS_NUM];

    /// Кнопки новые
    TriggerControl  button_call_driver_assistant[CABS_NUM];
    TriggerControl  button_check_edt[CABS_NUM];
    TriggerControl  button_check_fire_alarm[CABS_NUM];
    TriggerControl  button_find_troubles[CABS_NUM];
    TriggerControl  reset_set_protections[CABS_NUM];
    TriggerControl  button_empty1[CABS_NUM];
    TriggerControl  button_empty2[CABS_NUM];

    enum
    {
        NUM_RB = 3,
        RBS = 0,
        RB = 1,
        RBP = 2
    };

    /// Рукоятки бдительности
    TriggerControl rb[CABS_NUM][NUM_RB];

    /// Переключатель тормозного усилия
    SwitcherControl brake_force_switch[CABS_NUM];    

    /// Контроллер машиниста
    ControllerKM2202*   km[CABS_NUM] = {nullptr, nullptr};

    /// Аккумуляторная батарея
    Battery*            battery = nullptr;

    /// Контактор топливного насоса (КТН)
    Relay*              kontaktor_fuel_pump = nullptr;

    /// Топливный бак
    FuelTank*           fuel_tank = nullptr;

    /// Электрический топливный насос (ЭНТ)
    ElectricFuelPump*   electro_fuel_pump = nullptr;

    /// Дизель
    Disel*              disel = nullptr;

    /// Реле РУ8
    Relay*              ru8 = nullptr;

    /// Контактор маслопрокачивающего насоса (КМН)
    Relay*              kontaktor_oil_pump = nullptr;

    /// Реле времени прокачки масла
    TimeRelay*          oilpump_time_relay = nullptr;

    /// Реле времени прокрутки стартера
    TimeRelay*          starter_time_relay = nullptr;

    /// Электрический маслопрокачивающий насос (ЭМН)
    ElectricOilPump*    electro_oil_pump = nullptr;

    /// Стратер-генератор
    StarterGenerator*   starter_generator = nullptr;

    /// Контактор стартер-генератора (КД)
    Relay*              kontaktor_starter = nullptr;

    /// Реле РУ10
    Relay*              ru10 = nullptr;

    /// Реле РУ6
    Relay*              ru6 = nullptr;

    /// Реле РУ42
    Relay*              ru42 = nullptr;

    /// Реле РУ7
    Relay*              ru7 = nullptr;

    /// Реле РУ15
    Relay*              ru15 = nullptr;

    /// Блок-магнит МВ6
    Relay*              mv6 = nullptr;

    /// Вентиль топливных насосов (ВТН)
    Relay*              vtn = nullptr;

    /// Реле РУ4
    Relay*              ru4 = nullptr;

    enum
    {
        RU4_KVG_POWER,
        NUM_RU4_CONTACTS
    };

    /// Реле времени РВ4
    TimeRelay*          rv4 = nullptr;

    /// Реле времени РВ9
    TimeRelay*          rv9 = nullptr;

    /// Контактор регулятора напряжения
    Relay*              krn = nullptr;

    /// Регулятор напряжения заряда АКБ
    VoltageRegulator*   voltage_regulator = nullptr;

    /// Мотор-компрессор
    TEP70MotorCompressor* motor_compressor = nullptr;

    /// Регулятор давления в ГР
    PressureRegulator*  press_reg = nullptr;

    /// Главный резервуар
    Reservoir*          main_reservoir = nullptr;

    /// Концевой кран питательной магистрали спереди
    PneumoAngleCock*    anglecock_fl_fwd = nullptr;

    /// Концевой кран питательной магистрали сзади
    PneumoAngleCock*    anglecock_fl_bwd = nullptr;

    /// Рукав питательной  магистрали спереди
    PneumoHose*         hose_fl_fwd = nullptr;

    /// Рукав питательной  магистрали сзади
    PneumoHose*         hose_fl_bwd = nullptr;

    /// Реле РУ18
    Relay*              ru18 = nullptr;

    /// Контактор мотор-компрессора КТК1
    Relay*              ktk1 = nullptr;

    /// Контактор мотор-компрессора КТК2
    Relay*              ktk2 = nullptr;

    /// Реле времени РВ6
    TimeRelay*          rv6 = nullptr;

    /// Блокировочное устройство УБТ усл.№367м
    PneumoBrakeLock*    brake_lock[CABS_NUM] = {nullptr, nullptr};

    /// Поездной кран машиниста усл.№395
    BrakeCrane*         brake_crane[CABS_NUM] = {nullptr, nullptr};

    /// Кран впомогательного тормоза усл.№254
    LocoCrane*          loco_crane[CABS_NUM] = {nullptr, nullptr};

    /// ЭПК автостопа
    AutoTrainStop*      epk[CABS_NUM] = {nullptr, nullptr};

    /// УКБМ
    SafetyDevice*       safety_device[CABS_NUM] = {nullptr, nullptr};

    /// Тормозная магистраль
    Reservoir*          brakepipe = nullptr;

    /// Воздухораспределитель
    AirDistributor*     air_dist = nullptr;

    /// Электровоздухораспределитель
    ElectroAirDistributor* electro_air_dist = nullptr;

    /// Запасный резервуар
    Reservoir*          supply_reservoir = nullptr;

    /// Концевой кран тормозной магистрали спереди
    PneumoAngleCock*    anglecock_bp_fwd = nullptr;

    /// Концевой кран тормозной магистрали сзади
    PneumoAngleCock*    anglecock_bp_bwd = nullptr;

    /// Рукав тормозной магистрали спереди
    PneumoHoseEPB*      hose_bp_fwd = nullptr;

    /// Рукав тормозной магистрали сзади
    PneumoHoseEPB*      hose_bp_bwd = nullptr;

    /// Переключательный клапан магистрали тормозных цилиндров ЗПК
    SwitchingValve*     bc_switch_valve = nullptr;

    /// Тройник для распределения воздуха от переключательного клапана
    /// к тележкам
    PneumoSplitter*     bc_splitter = nullptr;

    enum
    {
        NUM_TROLLEYS = 2,
        NUM_AXIS_PER_TROLLEY = 3,
        TROLLEY_FWD = 0,
        TROLLEY_BWD = 1
    };

    /// Повторительное реле давления усл.№304
    std::array<PneumoRelay*, NUM_TROLLEYS> bc_pressure_relay = {nullptr, nullptr};

    /// Тормозные механизмы тележек
    std::array<BrakeMech*, NUM_TROLLEYS> brake_mech = {nullptr, nullptr};

    /// Концевой кран магистрали тормозных цилиндров спереди
    PneumoAngleCock*    anglecock_bc_fwd = nullptr;

    /// Концевой кран магистрали тормозных цилиндров сзади
    PneumoAngleCock*    anglecock_bc_bwd = nullptr;

    /// Рукав магистрали тормозных цилиндров спереди
    PneumoHose*         hose_bc_fwd = nullptr;

    /// Рукав магистрали тормозных цилиндров сзади
    PneumoHose*         hose_bc_bwd = nullptr;

    /// Источник питания ЭПТ
    EPBConverter*       epb_converter[CABS_NUM] = {nullptr, nullptr};

    /// Блок управления двухпроводного ЭПТ
    EPBControl*         epb_control[CABS_NUM] = {nullptr, nullptr};

    /// Возбудитель главного генератора
    FieldGenerator*     field_gen = nullptr;

    /// Контактор возбуждения возбудителя (КВВ)
    Relay*              kvv = nullptr;

    enum
    {
        KVV_CONTACT_MAIN,
        KVV_CONTACT_POWER_RESET,
        KVV_SELF_POWER,
        NUM_KVV_CONTACTS
    };

    /// Контактор возбуждения генератора (КВГ)
    Relay*              kvg = nullptr;

    enum
    {
        KVG_TRAC_GEN_FIELD_ON,
        KVG_VTN_OFF,
        KVG_FIELD_REG_ON,
        KVG_FOR_RU28_ON,
        NUM_KVG_CONTACTS
    };

    /// Главный (тяговый) генератор
    TracGenerator*      trac_gen = nullptr;

    /// Регулятор возбуждения тягового генератора
    FieldRegulator*     field_reg = nullptr;

    /// Ток, потребляемый от главного генератора
    double              I_gen = 0.0;

    /// Регистратор, для постоения графиков
    Registrator*        reg = nullptr;

    /// Скоростемер
    SL2M*               speed_meter[CABS_NUM] = {nullptr, nullptr};

    /// Напряжение аккумулятороной батареи
    double U_bat = 0.0;

    /// Напряжение цепей управления
    double  Ucc = 0.0;

    /// Ток цепей управления
    double  Icc = 0.0;

    /// Передаточное число тягового редуктора
    double  ip = 3.12;

    /// Контактор шунта 1 (КШ1)
    Relay*  ksh1 = nullptr;

    /// Контактор шунта 2 (КШ2)
    Relay*  ksh2 = nullptr;

    /// Реле управления РУ1
    Relay*  ru1 = nullptr;

    /// Свисток и тифон
    TrainHorn*          horn = nullptr;

    /// Система подачи песка
    SandingSystem*      sand_system = nullptr;

    double  tracForce = 0.0;

    /// Реверсор
    Reversor*           reversor = nullptr;

    /// Тормозной переключатель
    BrakeSwitcher*      brake_switcher = nullptr;

    /// Реле перехода РП1
    HysteresisRelay*    rp1 = nullptr;

    /// Реле перехода PП2
    HysteresisRelay*    rp2 = nullptr;

    /// Реле времени для выдержки включения КШ2 (нет в схеме!)
    TimeRelay*          ksh2_delay = nullptr;

    /// Реле выдержки времени для предотвращение отключения КШ1
    /// при выключении КШ2 (нет в схеме!)
    TimeRelay*          ksh1_delay = nullptr;

    enum
    {
        NUM_MOTORS = 6
    };

    /// Тумблеры отключателей тяговых двигателей (ОМ1-ОМ6)
    TriggerControl  tumbler_om[NUM_MOTORS];

    /// Переключатели ВВК
    enum
    {
        VVK_PA1 = 0,
        VVK_PA2,
        VVK_KB1,
        VVK_KB2,
        VVK_RC1,
        VVK_RC2,
        VVK_SW_NUM
    };

    TriggerControl  vvk_sw[VVK_SW_NUM];

    /// Контактные группы КБ1
    LogicRelay *KB1_switch = nullptr;

    enum
    {
        KB1_CONTACT_CAB1_K2_AB4,
        KB1_CONTACT_CAB1_K4_AB2,
        KB1_CONTACT_CAB1_K6,
        KB1_CONTACT_CAB1_K8,

        KB1_CONTACT_CAB2_K10_AB4,
        KB1_CONTACT_CAB2_K12_AB2,
        KB1_CONTACT_CAB2_K14,
        KB1_CONTACT_CAB2_K16,

        KB1_NUM_CONTACTS
    };

    /// Контактные группы КБ2
    LogicRelay *KB2_switch = nullptr;

    enum
    {
        KB2_CONTACT_CAB1_K1,
        KB2_CONTACT_CAB1_K3,
        KB2_CONTACT_CAB1_K5,
        KB2_CONTACT_CAB1_K7,

        KB2_CONTACT_CAB2_K9,
        KB2_CONTACT_CAB2_K11,
        KB2_CONTACT_CAB2_K13,
        KB2_CONTACT_CAB2_K15,

        KB2_NUM_CONTACTS
    };

    /// Тумблер числа тепловозов
    TriggerControl  tumbler_loco_count;

    /// Рубильник аккумуляторной батареи
    TriggerControl  accum_switch;

    /// Ручки дверей кабин
    TriggerControl  cabine1_door_handle;
    TriggerControl  cabine2_door_handle;

    /// Тумблер отопления кабины
    TriggerControl  tumbler_heater[CABS_NUM];

    /// АЗВ вентиляции кабины
    TriggerControl  azv_ventilation[CABS_NUM];

    /// Тумблер освещения кабины (собственный, независимо от АЗВ)
    TriggerControl  tumbler_cabine_light[CABS_NUM];

    // --- Охлаждение ---
    /// Гидропривод вентилятора горячего контура
    HydrostaticFanDrive* fan_hot = nullptr;
    /// Гидропривод вентилятора холодного контура
    HydrostaticFanDrive* fan_cold = nullptr;
    /// Аэродинамическая нагрузка вентилятора горячего контура
    CoolerFan* fan_hot_aero = nullptr;
    /// Аэродинамическая нагрузка вентилятора холодного контура
    CoolerFan* fan_cold_aero = nullptr;
    /// Вентиль жалюзи воды (ВЖВ)
    PneumoElectroValve* vzhv = nullptr;
    /// Вентиль жалюзи масла (ВЖМ)
    PneumoElectroValve* vzhm = nullptr;
    /// Термореле РТВ (жалюзи воды)
    HysteresisRelay* rtv = nullptr;
    /// Термореле РТМ (жалюзи масла)
    HysteresisRelay* rtm = nullptr;

    /// Термореле вентилятора горячего контура
    HysteresisRelay* rtv_fan = nullptr;
    /// Термореле вентилятора холодного контура
    HysteresisRelay* rtm_fan = nullptr;

    /// Состояние жалюзи для анимации
    bool water_blinds_open = false;
    bool oil_blinds_open = false;

    /// Тяговые двигатели
    std::array<TractionMotor*, NUM_MOTORS> motor = {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr};

    /// Поездные контакторы
    std::array<Relay*, NUM_MOTORS + 1> kp = {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr};

    enum
    {
        KP1,
        KP2,
        KP3,
        KP4,
        KP5,
        KP6,
        KP7,
        NUM_KP
    };

    enum
    {
        KP_POWER_ON,
        KP_OFF,
        KP_ON,
        NUM_KP_CONTACTS
    };

    /// Ограничения скорости на путевой инфраструктуре для кабины А
    SpeedMap*           speedmap_fwd = nullptr;
    /// Ограничения скорости на путевой инфраструктуре для кабины Б
    SpeedMap*           speedmap_bwd = nullptr;

    /// Приёмная катушка АЛСН для кабины А
    CoilALSN*           coil_ALSN_fwd = nullptr;
    /// Приёмная катушка АЛСН для кабины Б
    CoilALSN*           coil_ALSN_bwd = nullptr;

    /// Дешифратор сигнала АЛСН
    DecoderALSN*        alsn_decoder[CABS_NUM] = {nullptr, nullptr};

    enum
    {
        NUM_RU9_CONTACS = 1,
        RU9_EPK_CTRL = 0
    };

    /// Реле контроля сбора схемы от ЭПК
    Relay*              ru9 = new Relay(NUM_RU9_CONTACS);

    enum
    {
        RU21_TRAC_ON,
        RU21_BRAKE_ON,
        RU21_FOR_RV8_ON,
        RU21_FOR_RV2_ON,
        NUM_RU21_CONTACTS
    };

    /// Реле РУ21 (в схеме реостатного тормоза!!!)
    Relay*              ru21 = new Relay(NUM_RU21_CONTACTS);

    /// Контактор КТ (тормозной контактор)
    Relay* kt = nullptr;

    enum
    {
        KT_RESET_POWER,
        KT_TRACTION_ALLOW,
        KT_FOR_RV8_ON,
        KT_FOR_RV10_ON,
        KT_EBCU_ON,
        KT_712_ON,        
        NUM_KT_CONTACTS
    };

    bool is_AB2_CAB1_on = false;

    bool is_AB2_CAB2_on = false;

    bool is_AB4_CAB1_on = false;

    bool is_AB4_CAB2_on = false;

    bool is_Revers_Forward = false;

    bool is_Revers_Backward = false;

    /// Реле контроля температуры
    Relay *ru3 = nullptr;

    enum
    {
        RU3_TRACTION_ALLOW,
        RU3_TEMP_LAMP,
        NUM_RU3_CONTACTS
    };

    float water_temp_gauge_power[CABS_NUM] = { 0.0f };

    float oil_temp_gauge_power[CABS_NUM] = { 0.0f };

    float fuel_press_gauge_power[CABS_NUM] = { 0.0f };

    float oil_press_gauge_power[CABS_NUM] = { 0.0f };

    void stepGaugesPower(double t, double dt);

    /// Чтение конфигурационного файла
    void loadConfig(QString cfg_path) override;

    /// Инициализация всех систем тепловоза
    void initialization() override;

    /// Инициализация сцепных устройств
    void initCouplings(const QString& modules_dir, const QString& custom_cfg_dir);

    /// Инициализация цепей управления
    void initControlCircuit(const QString& modules_dir, const QString& custom_cfg_dir);

    /// Инициализация топливной системы
    void initFuelSystem(const QString& modules_dir, const QString& custom_cfg_dir);

    /// Инициализация дизеля
    void initDisel(const QString& modules_dir, const QString& custom_cfg_dir);

    /// Инициализация маслянной системы
    void initOilSystem(const QString& modules_dir, const QString& custom_cfg_dir);

    /// Инициализация питательной магистрали
    void initPneumoSupply(const QString& modules_dir, const QString& custom_cfg_dir);

    /// Инициализация приборов управления тормозами
    void initBrakesControl(const QString& modules_dir, const QString& custom_cfg_dir);

    /// Инициализация тормозного оборудования
    void initBrakesEquipment(const QString& modules_dir, const QString& custom_cfg_dir);

    /// Инициализация ЭПТ
    void initEPB(const QString& modules_dir, const QString& custom_cfg_dir);

    /// Инициализация электрической передачи
    void initElectroTransmission(const QString& modules_dir, const QString& custom_cfg_dir);

    /// Инициализация устройств безопасности
    void initSafetyDevices(const QString& modules_dir, const QString& custom_cfg_dir);

    /// Инициализация прочего оборудования
    void initOther(const QString& modules_dir, const QString& custom_cfg_dir);

    /// Инициализация системы охлаждения
    void initCooler(const QString& modules_dir, const QString& custom_cfg_dir);

    /// Инициализация управления
    void initControl(const QString& modules_dir, const QString& custom_cfg_dir);

    /// Инициализация аппаратов цепей ЭДТ
    void initElectrodynamicBrake(const QString& modules_dir, const QString& custom_cfg_dir);

    /// Процесс симуляции
    void process(const simulator_time_t& t, const double& dt) override;    

    /// Обработка сигналов от IOController
    void stepControls(double t, double dt);

    /// Отладочная строка
    void debugPrint(const simulator_time_t& t, const double& dt);

    /// Сигналы для анимации
    void signalsOutput(const simulator_time_t& t, const double& dt);

    /// Сигналы для озвучки
    void soundsOutput(const simulator_time_t& t, const double& dt);

    /// Предварительные расчёты перед симуляцией
    void preStep(const double& t) override;

    /// Предварительный расчёт координат сцепных устройств
    void preStepCouplings(const double& t);

    /// Шаг моделирования всех систем локомотива в целом
    void step(const double& t, const double& dt) override;

    /// Шаг моделирования сцепных устройств
    void stepCouplings(const double& t, const double& dt);

    /// Шаг моделирования цепей управления
    void stepControlCircuit(const double& t, const double& dt);

    /// Шаг моделирования топливной системы
    void stepFuelSystem(const double& t, const double& dt);

    /// Шаг моделирования дизеля
    void stepDisel(const double& t, const double& dt);

    /// Шаг моделирования масляной системы
    void stepOilSystem(const double& t, const double& dt);

    /// Шаг моделирования питательной магистрали
    void stepPneumoSupply(const double& t, const double& dt);

    /// Шаг моделирования приборов управления тормозами
    void stepBrakesControl(const double& t, const double& dt);

    /// Шаг моделирования тормозного оборудования
    void stepBrakesEquipment(const double& t, const double& dt);

    /// Шаг моделирования ЭПТ
    void stepEPB(const double& t, const double& dt);

    /// Шаг моделирования электрической передачи
    void stepElectroTransmission(const double& t, const double& dt);

    /// Шаг моделирования устройств безопасности
    void stepSafetyDevices(const double& t, const double& dt);

    /// Шаг моделирования прочего оборудования
    void stepOther(const double& t, const double& dt);

    /// Шаг моделирования системы охлаждения
    void stepCooler(const double& t, const double& dt);

    /// Шаг моделирования цепей ЭДТ
    void stepElectroDymanicBrake(const double &t, const double &dt);

    /// Привод жалюзи ЭДТ
    BlindsActor *EDB_blinds_actor = nullptr;

    /// Реле РУ37
    Relay *ru37 = nullptr;

    enum
    {
        RU37_EDB_BLINGS_LAMP,
        RU37_FOR_RV2_ON,
        NUM_RU37_CONTACTS
    };

    /// Реле РУ26
    Relay *ru26 = nullptr;

    enum
    {
        RU26_FOR_RU25_ON,
        RU26_FOR_KT_ON,
        NUM_RU26_CONTACTS
    };

    /// Реле РУ27
    Relay *ru27 = nullptr;

    enum
    {
        RU27_FOR_RU24_ON,
        RU27_TRACTION_ALLOW,
        NUM_RU27_CONTACTS
    };

    /// Реле РУ24
    Relay *ru24 = nullptr;

    enum
    {
        RU24_FIELD_REG_ON,
        RU24_FOR_RU25_ON,
        RU24_FOR_RU22_ON,
        RU24_FOR_MP4_ON,
        RU24_FOR_RV11_OFF,
        RU24_FOR_RV10_OFF,
        NUM_RU24_CONTACTS
    };

    /// Реле РУ20
    Relay *ru20 = nullptr;

    enum
    {
        RU20_FOR_RU25_ON,
        NUM_RU20_CONTACTS
    };

    /// Реле РУ23
    Relay *ru23 = nullptr;

    bool is_RU23_ON = false;

    enum
    {
        RU23_FOR_RU25_OFF,
        NUM_RU23_CONTACTS
    };

    /// Реле РУ25
    Relay *ru25 = nullptr;

    bool is_RU25_ON = false;

    enum
    {
        RU25_KVG_POWER,
        RU25_FOR_RU21_ON,
        RU25_FOR_VT1_ON,
        NUM_RU25_CONTACTS
    };

    /// Реле времени РВ2
    TimeRelay *rv2 = nullptr;

    enum
    {
        RV2_KP_ON_OFF,
        NUM_RV2_CONTACTS
    };

    /// Реле времени РВ8
    TimeRelay *rv8 = nullptr;

    enum
    {
        RV8_KP7_ON_OFF,
        NUM_RV8_CONTACTS
    };

    /// Реле РУ22
    Relay *ru22 = nullptr;

    enum
    {
        RU22_MP1_ON,
        RU22_MP2_ON,
        RU22_MP3_ON,
        RU22_MP4_ON,
        RU22_MP1_OFF,
        RU22_MP2_OFF,
        RU22_MP3_OFF,
        RU22_MP4_OFF,
        RU22_FOR_RV12_ON,
        NUM_RU22_CONTACTS
    };

    enum
    {
        MP1,
        MP2,
        MP3,
        MP4,
        NUM_MP
    };

    /// Блок-контакты управления честотой вращения дизеля
    Relay *mp[NUM_MP] = { nullptr };

    enum
    {
        MP_STATE,
        NUM_MP_STATES
    };

    /// Провод 728
    bool is_728_ON = false;

    /// Реле РУ32
    Relay *ru32 = nullptr;

    enum
    {
        RU32_FOR_RV2_ON,
        NUM_RU32_CONTACTS
    };

    /// Блок управления электрическим тормозом (БЭТ)
    ControlUnitEDB *edb_control_unit = nullptr;

    /// Вспомогательный тормозной контактор КТ1
    Relay *kt1 = nullptr;

    enum
    {
        KT1_FOR_KP7_ON,
        KT1_TRACTION_ALLOW,
        NUM_KT1_CONTACTS
    };

    bool is_777_ON = false;

    /// Реле времени РВ12 увеличения оборотов дизеля при ЭДТ
    TimeRelay *rv12 = nullptr;

    enum
    {
        RV12_MP2_ON,
        RV12_MP3_ON,
        NUM_RV12_CONTACTS
    };

    /// Реле РУ28
    Relay *ru28 = nullptr;

    enum
    {
        RU28_FOR_RV12_OFF,
        RU28_FOR_RU25_ON,
        RU28_FOR_LS5_ON,
        RU28_FOR_RV11_ON,
        NUM_RU28_CONTACTS
    };

    /// Реле РУ29
    Relay *ru29 = nullptr;

    enum
    {
        RU29_FOR_RU28_OFF,
        NUM_RU29_CONTACTS
    };

    /// Реле времени РВ11 (реле предварительного торможения)
    TimeRelay *rv11 = nullptr;

    enum
    {
        RV11_FULL_BRAKE_FORCE,
        NUM_RV11_CONTACTS
    };

    /// Реле времени РВ10 (контроль эффективности торможения)
    TimeRelay *rv10 = nullptr;

    enum
    {
        RV10_FOR_RU23_ON,
        NUM_RV10_CONTACTS
    };

    /// Реле РМн1 (контроль минимальной величины тормозного тока)
    Relay *rmn1 = nullptr;

    enum
    {
        RMn1_FOR_RU23_OFF,
        NUM_RMn1_CONTACTS
    };

    /// Веса блок-контактов при перемещении регулятора оборотов
    double mp_weight[NUM_MP] = {2, 3.97, 7.91, -1};

    float cab_light_signal[CABS_NUM] = {0.0f, 0.0f};

    float gauges_light_signal[CABS_NUM] = {0.0f, 0.0f};

    float spotlight_signal[CABS_NUM] = {0.0f, 0.0f};

    float trolley_light_signal = 0.0f;

    enum
    {
        BUF_RED,
        BUF_WHITE,
        NUM_BUF_LIGHTS = 2
    };

    using buf_light_signals_t = std::array<float, NUM_BUF_LIGHTS>;

    buf_light_signals_t buf_lights_left[CABS_NUM];

    buf_light_signals_t buf_lights_right[CABS_NUM];

    float power_level = 0.0f;

    /// Шаг моделирования освещения
    void stepLighting(const double &t, const double &dt);

    enum
    {
        LAMP_BATTARY_CHARGE,
        LAMP_OIL_PRESSURE,
        LAMP_WATER_OIL_TEMP,
        LAMP_POWER_RESET,
        LAMP_EDT_BLINDS_OPEN_1,
        LAMP_EDT_BLINDS_OPEN_2,
        LAMP_RESERVED_FUEL_PUMP,
        LAMP_EDB_BLINDS_OPEN,
        LAMP_EDB_READY_1,
        LAMP_EDB_READY_2,
        NUM_LAMPS
    };

    using control_lamps_t = std::array<float, NUM_LAMPS>;

    control_lamps_t control_lamps[CABS_NUM];

    /// Симуляция контрольных ламп, с учетом питания и блокировок
    void stepControlLamps(const double &t, const double &dt);

    /// Формирование состояния сигнальных ламп
    float getLampState(int cab_idx, double signal);

    void load_brakes_config(QString path);

    Timer *autoStartTimer = new Timer(0.5, false);

    std::vector<Trigger *> autostart_triggers;
    size_t start_count = 0;
    size_t autostart_cab = 0;

    /// Сохранённое перед автостартом состояние органов управления
    struct autostart_saved_state_t
    {
        std::uint16_t field_weak1_pos = 0;
        std::uint16_t field_weak2_pos = 0;
        std::int8_t   km_main_shaft = 0;
        std::int8_t   km_revers = 0;
        bool          brake_lock_on = false;
    };

    autostart_saved_state_t autostart_saved_state[CABS_NUM];
    bool autostart_state_saved[CABS_NUM] = {false, false};

    /// Режим работы процедуры автостарта/автоостанова
    enum AutostartMode
    {
        AS_MODE_START = 0,
        AS_MODE_STOP = 1
    };

    AutostartMode autostart_mode = AS_MODE_START;

    /// Признак работы дизеля в момент выключения АВ "Топливный насос"
    bool stop_diesel_was_running = false;
    /// Признак наблюдаемой прокачки масла после остановки дизеля
    bool stop_recirculation_seen = false;
    /// Счётчик тиков ожидания завершения прокачки масла (страховка от зависания)
    int stop_wait_ticks = 0;

    TriggerControl autopilot_switcher[CABS_NUM];

    tep70_control_t *auto_control[CABS_NUM] = {nullptr, nullptr};

    tep70_feedback_t *auto_feedback[CABS_NUM];

    double charge_press = 0.0;

    bool is_DP_closed = false;

    bool is_819_ON = false;

    bool is_KP_on = false;

    bool is_KVG_on = false;

    bool is_KVV_on = false;

    HandbrakeActor *handbrake_actor = nullptr;

    /// Вентиль отпуска тормозов при работе ЭДТ (ВТ1)
    ElectroPneumaticValve* vt1 = nullptr;

    /// Переключательный клапан между каналом дотормаживания и ВР/ЭВР
    SwitchingValve *pk4 = nullptr;

    /// Редуктор РЕД1 питания цепи дотормаживания
    PneumoReducer *redc1 = nullptr;

    /// Резервуар РС3 в цепи дотормаживания
    Reservoir *rs3 = nullptr;

    /// Проверка активной кабины
    int checkActiveCabine();

    bool initAutostartProgram(int cab_autostart_request);

    bool initAutostopProgram(int cab_autostop_request);

    /// Проверка завершения прокачки масла после остановки дизеля
    bool isOilRecirculationFinished();

    void initAutopilot(const QString& modules_dir, const QString& custom_cfg_dir);

    void stepAutopilot(double t, double dt);

    void prepareCabineForAutopilot(int my_cab_idx, int other_cab_idx);

    void OnAutopilot() override;

    void OffAutopilot() override;

    TriggerControl shunting_mode_switcher[CABS_NUM];

private slots:

    void slotAutostart();

    void slotAutostop();

    void slotInitTrainForAutopilot();
};

#endif // TEP70_H
