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

#include    "vehicle-api.h"
#include    "tep70-signals.h"

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
#include    "sl2m.h"
#include    "km-2202.h"
#include    "reversor.h"
#include    "brake-switcher.h"

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
    void initBrakeDevices(double p0, double pBP, double pFL);

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

    /// Контроллер машиниста
    ControllerKM2202    *km = nullptr;

    /// Аккумуляторная батарея
    Battery             *battery = nullptr;

    /// Контактор топливного насоса (КТН)
    Relay               *kontaktor_fuel_pump = nullptr;

    /// Топливный бак
    FuelTank            *fuel_tank = nullptr;

    /// Электрический топливный насос (ЭНТ)
    ElectricFuelPump    *electro_fuel_pump = nullptr;

    /// Дизель
    Disel               *disel = nullptr;

    /// Реле РУ8
    Relay               *ru8 = nullptr;

    /// Контактор маслопрокачивающего насоса (КМН)
    Relay               *kontaktor_oil_pump = nullptr;

    /// Реле времени прокачки масла
    TimeRelay           *oilpump_time_relay = nullptr;

    /// Реле времени прокрутки стартера
    TimeRelay           *starter_time_relay = nullptr;

    /// Электрический маслопрокачивающий насос (ЭМН)
    ElectricOilPump     *electro_oil_pump = nullptr;

    /// Стратер-генератор
    StarterGenerator    *starter_generator = nullptr;

    /// Контактор стартер-генератора (КД)
    Relay               *kontaktor_starter = nullptr;

    /// Реле РУ10
    Relay               *ru10 = nullptr;

    /// Реле РУ6
    Relay               *ru6 = nullptr;

    /// Реле РУ42
    Relay               *ru42 = nullptr;

    /// Реле РУ7
    Relay               *ru7 = nullptr;

    /// Реле РУ15
    Relay               *ru15 = nullptr;

    /// Блок-магнит МВ6
    Relay               *mv6 = nullptr;

    /// Вентиль топливных насосов (ВТН)
    Relay               *vtn = nullptr;

    /// Реле РУ4
    Relay               *ru4 = nullptr;

    /// Реле времени РВ4
    TimeRelay           *rv4 = nullptr;

    /// Реле времени РВ9
    TimeRelay           *rv9 = nullptr;

    /// Контактор регулятора напряжения
    Relay               *krn = nullptr;

    /// Регулятор напряжения заряда АКБ
    VoltageRegulator    *voltage_regulator = nullptr;

    /// Мотор-компрессор
    TEP70MotorCompressor *motor_compressor = nullptr;

    /// Регулятор давления в ГР
    PressureRegulator   *press_reg = nullptr;

    /// Главный резервуар
    Reservoir           *main_reservoir = nullptr;

    /// Концевой кран питательной магистрали спереди
    PneumoAngleCock     *anglecock_fl_fwd = nullptr;

    /// Концевой кран питательной магистрали сзади
    PneumoAngleCock     *anglecock_fl_bwd = nullptr;

    /// Рукав питательной  магистрали спереди
    PneumoHose          *hose_fl_fwd = nullptr;

    /// Рукав питательной  магистрали сзади
    PneumoHose          *hose_fl_bwd = nullptr;

    /// Реле РУ18
    Relay               *ru18 = nullptr;

    /// Контактор мотор-компрессора КТК1
    Relay               *ktk1 = nullptr;

    /// Контактор мотор-компрессора КТК2
    Relay               *ktk2 = nullptr;

    /// Реле времени РВ6
    TimeRelay           *rv6 = nullptr;

    /// Блокировочное устройство УБТ усл.№367м
    BrakeLock           *brake_lock = nullptr;

    /// Поездной кран машиниста усл.№395
    BrakeCrane          *brake_crane = nullptr;

    /// Кран впомогательного тормоза усл.№254
    LocoCrane           *loco_crane = nullptr;

    /// ЭПК автостопа
    AutoTrainStop       *epk = nullptr;

    /// Тормозная магистраль
    Reservoir           *brakepipe = nullptr;

    /// Воздухораспределитель
    AirDistributor      *air_dist = nullptr;

    /// Электровоздухораспределитель
    ElectroAirDistributor  *electro_air_dist = nullptr;

    /// Запасный резервуар
    Reservoir           *supply_reservoir = nullptr;

    /// Концевой кран тормозной магистрали спереди
    PneumoAngleCock     *anglecock_bp_fwd = nullptr;

    /// Концевой кран тормозной магистрали сзади
    PneumoAngleCock     *anglecock_bp_bwd = nullptr;

    /// Рукав тормозной магистрали спереди
    PneumoHoseEPB       *hose_bp_fwd = nullptr;

    /// Рукав тормозной магистрали сзади
    PneumoHoseEPB       *hose_bp_bwd = nullptr;

    /// Переключательный клапан магистрали тормозных цилиндров ЗПК
    SwitchingValve      *bc_switch_valve = nullptr;

    /// Тройник для распределения воздуха от переключательного клапана
    /// к тележкам
    PneumoSplitter      *bc_splitter = nullptr;

    enum
    {
        NUM_TROLLEYS = 2,
        NUM_AXIS_PER_TROLLEY = 3,
        TROLLEY_FWD = 0,
        TROLLEY_BWD = 1
    };

    /// Повторительное реле давления усл.№304
    std::array<PneumoRelay *, NUM_TROLLEYS> bc_pressure_relay = {nullptr, nullptr};

    /// Тормозные механизмы тележек
    std::array<BrakeMech *, NUM_TROLLEYS> brake_mech = {nullptr, nullptr};

    /// Концевой кран магистрали тормозных цилиндров спереди
    PneumoAngleCock     *anglecock_bc_fwd = nullptr;

    /// Концевой кран магистрали тормозных цилиндров сзади
    PneumoAngleCock     *anglecock_bc_bwd = nullptr;

    /// Рукав магистрали тормозных цилиндров спереди
    PneumoHose          *hose_bc_fwd = nullptr;

    /// Рукав магистрали тормозных цилиндров сзади
    PneumoHose          *hose_bc_bwd = nullptr;

    /// Источник питания ЭПТ
    EPBConverter        *epb_converter = nullptr;

    /// Блок управления двухпроводного ЭПТ
    EPBControl          *epb_control = nullptr;

    /// Возбудитель главного генератора
    FieldGenerator          *field_gen = nullptr;

    /// Контактор возбуждения возбудителя (КВВ)
    Relay                   *kvv = nullptr;

    /// Контактор возбуждения генератора (КВГ)
    Relay                   *kvg = nullptr;

    /// Главный (тяговый) генератор
    TracGenerator           *trac_gen = nullptr;

    /// Регулятор возбуждения тягового генератора
    FieldRegulator          *field_reg = nullptr;

    /// Ток, потребляемый от главного генератора
    double                  I_gen = 0.0;

    /// Регистратор, для постоения графиков
    Registrator             *reg = nullptr;

    /// Скоростемер
    SL2M                    *speed_meter = nullptr;

    /// Кнопка "Пуск дизеля"
    bool    button_disel_start = false;

    /// Кнопка "Отпуск тормозов"
    bool    button_brake_release = false;

    /// Кнопка "Свисток"
    bool    button_svistok = false;

    /// Кнопка "Тифон"
    bool    button_tifon = false;

    /// Рукоятка бдительности (РБ1)
    bool    button_RB1 = false;

    /// Напряжение цепей управления
    double  Ucc = 0.0;

    /// Ток цепей управления
    double  Icc = 0.0;

    /// Передаточное число тягового редуктора
    double  ip = 3.12;

    /// Контактор шунта 1 (КШ1)
    Relay   *ksh1 = nullptr;

    /// Контактор шунта 2 (КШ2)
    Relay   *ksh2 = nullptr;

    /// Реле управления РУ1
    Relay   *ru1 = nullptr;

    /// Свисток и тифон
    TrainHorn   *horn = nullptr;

    /// Система подачи песка
    SandingSystem   *sand_system = nullptr;

    double  tracForce = 0.0;

    bool    is_svistok = false;

    bool    is_tifon = false;

    /// Реверсор
    Reversor    *reversor = nullptr;

    /// Тормозной переключатель
    BrakeSwitcher *brake_switcher = nullptr;

    /// Реле перехода РП1
    HysteresisRelay     *rp1 = nullptr;

    /// Реле перехода PП2
    HysteresisRelay     *rp2 = nullptr;

    /// Реле времени для выдержки включения КШ2 (нет в схеме!)
    TimeRelay           *ksh2_delay = nullptr;

    /// Реле выдержки времени для предотвращение отключения КШ1
    /// при выключении КШ2 (нет в схеме!)
    TimeRelay           *ksh1_delay = nullptr;

    enum
    {
        NUM_MOTORS = 6
    };

    /// Тяговые двигатели
    std::array<TractionMotor *, NUM_MOTORS> motor = {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr};

    /// Поездные контакторы
    std::array<Relay *, NUM_MOTORS + 1> kp = {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr};

    /// Ограничения скорости на путевой инфраструктуре для кабины А
    SpeedMap    *speedmap_fwd = nullptr;
    /// Ограничения скорости на путевой инфраструктуре для кабины Б
    SpeedMap    *speedmap_bwd = nullptr;

    /// Приёмная катушка АЛСН для кабины А
    CoilALSN    *coil_ALSN_fwd = nullptr;
    /// Приёмная катушка АЛСН для кабины Б
    CoilALSN    *coil_ALSN_bwd = nullptr;

    /// Дешифратор сигнала АЛСН
    DecoderALSN *alsn_decoder = nullptr;

    /// АЗВ "Управление общее"
    Trigger azv_common_control;

    /// АЗВ "Управление тепловозом"
    Trigger azv_upr_tepl;

    /// АЗВ "Топливный насос"
    Trigger azv_fuel_pump;

    /// АЗВ "ЭДТ"
    Trigger azv_edt_on;

    /// АЗВ "Тормоз питание"
    Trigger azv_edt_power;

    /// АЗВ "ЭПТ"
    Trigger azv_ept_on;

    /// АЗВ "Компрессор"
    Trigger azv_motor_compressor;

    /// Тумблер "Напряжение ЦУ. Напряжение ЭПТ"
    Trigger tumbler_voltage;

    /// Тумблер "Аварийная остановка дизеля"
    Trigger tumbler_disel_stop;

    /// Тумблер "Ослабление поля I ступени руч./авт."
    Switcher tumbler_field_weak1;

    /// Тумблер "Ослабление поля II ступени руч./авт."
    Switcher tumbler_field_weak2;

    /// Тумблер "Управление жалюзи воды руч./авт."
    Switcher tumbler_water_zaluzi;

    /// Тумблер "Управление жалюзи масла руч./авт."
    Switcher tumbler_oil_zaluzi;

    /// Инициализация всех систем тепловоза
    void initialization();

    /// Инициализация сцепных устройств
    void initCouplings(const QString &modules_dir, const QString &custom_cfg_dir);

    /// Инициализация органов управления в кабине
    void initCabineControls(const QString &modules_dir, const QString &custom_cfg_dir);

    /// Инициализация цепей управления
    void initControlCircuit(const QString &modules_dir, const QString &custom_cfg_dir);

    /// Инициализация топливной системы
    void initFuelSystem(const QString &modules_dir, const QString &custom_cfg_dir);

    /// Инициализация дизеля
    void initDisel(const QString &modules_dir, const QString &custom_cfg_dir);

    /// Инициализация маслянной системы
    void initOilSystem(const QString &modules_dir, const QString &custom_cfg_dir);

    /// Инициализация питательной магистрали
    void initPneumoSupply(const QString &modules_dir, const QString &custom_cfg_dir);

    /// Инициализация приборов управления тормозами
    void initBrakesControl(const QString &modules_dir, const QString &custom_cfg_dir);

    /// Инициализация тормозного оборудования
    void initBrakesEquipment(const QString &modules_dir, const QString &custom_cfg_dir);

    /// Инициализация ЭПТ
    void initEPB(const QString &modules_dir, const QString &custom_cfg_dir);

    /// Инициализация электрической передачи
    void initElectroTransmission(const QString &modules_dir, const QString &custom_cfg_dir);

    /// Инициализация устройств безопасности
    void initSafetyDevices(const QString &modules_dir, const QString &custom_cfg_dir);

    /// Инициализация прочего оборудования
    void initOther(const QString &modules_dir, const QString &custom_cfg_dir);

    /// Предварительные расчёты перед симуляцией
    void preStep(double t);

    /// Предварительный расчёт координат сцепных устройств
    void preStepCouplings(double t);

    /// Шаг моделирования всех систем локомотива в целом
    void step(double t, double dt);

    /// Шаг моделирования сцепных устройств
    void stepCouplings(double t, double dt);

    /// Шаг моделирования органов управления в кабине
    void stepCabineControls(double t, double dt);

    /// Шаг моделирования цепей управления
    void stepControlCircuit(double t, double dt);

    /// Шаг моделирования топливной системы
    void stepFuelSystem(double t, double dt);

    /// Шаг моделирования дизеля
    void stepDisel(double t, double dt);

    /// Шаг моделирования масляной системы
    void stepOilSystem(double t, double dt);

    /// Шаг моделирования питательной магистрали
    void stepPneumoSupply(double t, double dt);

    /// Шаг моделирования приборов управления тормозами
    void stepBrakesControl(double t, double dt);

    /// Шаг моделирования тормозного оборудования
    void stepBrakesEquipment(double t, double dt);

    /// Шаг моделирования ЭПТ
    void stepEPB(double t, double dt);

    /// Шаг моделирования электрической передачи
    void stepElectroTransmission(double t, double dt);

    /// Шаг моделирования устройств безопасности
    void stepSafetyDevices(double t, double dt);

    /// Шаг моделирования прочего оборудования
    void stepOther(double t, double dt);

    /// Вывод сигналов для анимаций
    void stepSignalsOutput(double t, double dt);

    /// Вывод сигналов озвучки
    void stepSoundsSignals(double t, double dt);

    /// Работа локомотивного светофора по сигналам АЛСН
    void stepDecodeAlsn();

    /// Вывод отладочной строки
    void debugOutput(double t, double dt);

    /// Формирование состояния сигнальных ламп
    float getLampState(double signal);

    /// Загрузка данных из конфигурационных файлов
    void loadConfig(QString cfg_path);

    /// Обработка клавиш
    void keyProcess();
};

#endif // TEP70_H
