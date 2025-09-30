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
#ifndef     TEP70BS_H
#define     TEP70BS_H

#include    "vehicle-api.h"
#include    "tep70bs-signals.h"

#include    "fuel-tank.h"
#include    "electric-fuel-pump.h"
#include    "disel.h"
#include    "electric-oil-pump.h"
#include    "starter-generator.h"
#include    "voltage-regulator.h"
#include    "tep70-motor-compressor.h"
#include    "field-generator.h"
#include    "trac-generator.h"
#include    "field-regulator.h"
#include    "trac-motor.h"
#include    "km-2202.h"
#include    "reversor.h"
#include    "brake-switcher.h"
#include    "cabine-switcher.h"

#include    "msut.h"

#include    "registrator.h"

/*!
 * \class
 * \brief Основной класс, описывающий весь тепловоз
 */
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
class TEP70BS : public Vehicle
{
public:

    /// Конструктор
    TEP70BS();

    /// Деструктор
    ~TEP70BS();

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

    /// Сцепка спереди
    Coupling *coupling_fwd = nullptr;
    /// Сцепка сзади
    Coupling *coupling_bwd = nullptr;

    /// Расцепной рычаг спереди
    OperatingRod *oper_rod_fwd = nullptr;
    /// Расцепной рычаг сзади
    OperatingRod *oper_rod_bwd = nullptr;

    enum
    {
        /// Число кабин
        CABS_NUM = 2,
        /// Индекс 1 кабины
        CAB1 = 0,
        /// Индекс 2 кабины
        CAB2 = 1
    };

    /// АЗВ "Управление общее" (АВ2)
    TriggerControl  azv_common_control[CABS_NUM];

    /// АЗВ "Управление тепловозом" (АВ1)
    TriggerControl  azv_upr_tepl[CABS_NUM];

    /// АЗВ "Топливный насос" (АВ4)
    TriggerControl  azv_fuel_pump[CABS_NUM];

    /// АЗВ "ЭДТ"
    TriggerControl  azv_edt_on[CABS_NUM];

    /// АЗВ "Тормоз питание"
    TriggerControl  azv_edt_power[CABS_NUM];

    /// АЗВ "ЭПТ"
    TriggerControl  azv_ept_on[CABS_NUM];

    /// АЗВ "Компрессор"
    TriggerControl  azv_motor_compressor[CABS_NUM];

    /// Тумблер "Напряжение ЦУ. Напряжение ЭПТ"
    TriggerControl  tumbler_voltage[CABS_NUM];

    /// Тумблер "Аварийная остановка дизеля"
    TriggerControl  tumbler_disel_stop[CABS_NUM];

    /// Тумблер "Ослабление поля I ступени руч./авт."
    SwitcherControl tumbler_field_weak1[CABS_NUM];

    /// Тумблер "Ослабление поля II ступени руч./авт."
    SwitcherControl tumbler_field_weak2[CABS_NUM];

    /// Тумблер "Управление жалюзи воды руч./авт."
    SwitcherControl tumbler_water_zaluzi[CABS_NUM];

    /// Тумблер "Управление жалюзи масла руч./авт."
    SwitcherControl tumbler_oil_zaluzi[CABS_NUM];

    /// Переключатель тормозного усилия
    SwitcherControl brake_force_switch[CABS_NUM];

    /// Кнопка "Пуск дизеля"
    TriggerControl  button_disel_start[CABS_NUM];

    /// Кнопка "Отпуск тормозов"
    TriggerControl  button_brake_release[CABS_NUM];

    /// Кнопка "Свисток"
    TriggerControl  button_svistok[CABS_NUM];

    /// Кнопка "Тифон"
    TriggerControl  button_tifon[CABS_NUM];

    enum
    {
        NUM_RB = 3,
        RBS = 0,
        RB = 1,
        RBP = 2
    };

    /// Рукоятки бдительности
    TriggerControl rb[CABS_NUM][NUM_RB];

    /// Ключ ЭПК
    TriggerControl key_epk[CABS_NUM];

    SwitcherControl tumbler_revers[CABS_NUM];

    /// Переключатель-блокировка рабочей кабины
    CabineSwitcher* cabine_switcher = nullptr;

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
    PneumoBrakeLock     *brake_lock[CABS_NUM] = {nullptr, nullptr};

    /// Поездной кран машиниста усл.№395
    BrakeCrane*         brake_crane[CABS_NUM] = {nullptr, nullptr};

    /// Кран впомогательного тормоза усл.№254
    LocoCrane*          loco_crane[CABS_NUM] = {nullptr, nullptr};

    /// ЭПК автостопа
    AutoTrainStop*      epk[CABS_NUM] = {nullptr, nullptr};

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
    EPBConverter*       epb_converter = nullptr;

    /// Блок управления двухпроводного ЭПТ
    EPBControl*         epb_control = nullptr;

    /// Возбудитель главного генератора
    FieldGenerator*     field_gen = nullptr;

    /// Контактор возбуждения возбудителя (КВВ)
    Relay*              kvv = nullptr;

    /// Контактор возбуждения генератора (КВГ)
    Relay*              kvg = nullptr;

    /// Главный (тяговый) генератор
    TracGenerator*      trac_gen = nullptr;

    /// Регулятор возбуждения тягового генератора
    FieldRegulator*     field_reg = nullptr;

    /// Ток, потребляемый от главного генератора
    double              I_gen = 0.0;

    /// Регистратор, для постоения графиков
    Registrator*        reg = nullptr;

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

    /// Микропроцессорная система управления тепловозом
    MSUT*                msut = nullptr;

    enum
    {
        NUM_MOTORS = 6
    };

    /// Тяговые двигатели
    std::array<TractionMotor*, NUM_MOTORS> motor = {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr};

    /// Поездные контакторы
    std::array<Relay*, NUM_MOTORS + 1> kp = {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr};

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
        NUM_RU21_CONTACTS = 1,
        RU21_TRAC_ON = 0
    };

    /// Реле РУ21 (в схеме реостатного тормоза!!!)
    Relay*              ru21 = new Relay(NUM_RU21_CONTACTS);

    /// Состояние последовательной цепи размыкающих контактов КП1 - КП7
    bool is_KP1_KP7_off = false;

    /// Состояние последовательной цепи замыкающих контактов КП1 - КП6
    bool is_KP1_KP6_on = false;

    msut_input_t msut_input = msut_input_t();

    msut_output_t msut_output = msut_output_t();

    std::vector<Trigger*> triggers;

    size_t start_count = 0;

    Timer autoStartTimer;


    /// Чтение конфигурационного файла
    void loadConfig(QString cfg_path) override;


    /// Инициализация всех систем тепловоза
    void initialization() override;

    /// Инициализация сцепных устройств
    void initCouplings(const QString& modules_dir, const QString& custom_cfg_dir);

    /// Инициализация органов управления в кабине
    void initCabineControls(const QString& modules_dir, const QString& custom_cfg_dir);

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

    /// Инициализация МСУТ
    void initMSUT(const QString& modules_dir, const QString& custom_cfg_dir);

    /// Инициализация управления
    void initControl(const QString& modules_dir, const QString& custom_cfg_dir);

    /// Инициализация процедуры автозапуска
    void initAutostart();


    /// Процесс симуляции
    void process(const simulator_time_t& t, const double& dt) override;

    /// Управление
    void keyProcess(const simulator_time_t& t, const double& dt);

    /// Отладочная строка
    void debugPrint(const simulator_time_t& t, const double& dt);

    /// Сигналы для анимации
    void signalsOutput(const simulator_time_t& t, const double& dt);

    /// Вывод сигналов на дисплей МСУ-ТЭ
    void signalsMSUT(const simulator_time_t& t, const double& dt);

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

    /// Шаг моделирования органов управления в кабине
    void stepCabineControls(const double& t, const double& dt);

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

    /// Шаг работы МСУТ
    void stepMSUT(const double& t, const double& dt);

    void stepAutostart(const double& t, const double& dt);


    /// Формирование состояния сигнальных ламп
    float getLampState(double signal);

    void load_brakes_config(QString path);

private slots:

    void slotAutostart();
};

#endif // TEP70_H
