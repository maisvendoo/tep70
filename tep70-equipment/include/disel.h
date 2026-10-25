#ifndef     DISEL_H
#define     DISEL_H

#include    "device.h"

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
class Disel : public Device
{
public:

    Disel(QObject *parent = Q_NULLPTR);

    ~Disel();

    void step(double t, double dt);

    /// Задать расход масла от электрического маслопрокачивающего насоса (ЭМН)
    void setQ_emn(double Q_emn);

    /// Задать крутящий момент от стартер-генератора
    void setStarterTorque(double M_sg);

    /// Задать момент от возбудителя
    void setFGTorque(double M_fg) { this->M_fg = M_fg; }

    /// Задать момент от главного генератора
    void setGenTorque(double M_gen) { this->M_gen = M_gen; }

    /// Задать состояние МВ6
    void setMV6state(bool state_mv6) { this->state_mv6 = state_mv6; }

    /// Задать состояние ВНТ
    void setVTNstate(bool state_vtn) { this->state_vtn = state_vtn; }

    /// Задать давление топлива перед ТНВД
    void setFuelPressure(double fuel_pressure) { this->fuel_pressure = fuel_pressure; }

    /// Задать чатоту вращения коленчатого вала
    void setRefFreq(double n_ref)
    {
        this->n_ref_prev = this->n_ref;
        this->n_ref = n_ref;
    }

    /// Задать уровень топлива бака
    void setFuelLevel(double fuel_level)
    {
        this->fuel_level = fuel_level;
    }

    /// Вернуть давление в системе смазки
    double getOilPressure() const { return getY(0); }

    /// Вернуть частоту вращения привода стартер-генератора
    double getStarterOmega() const { return ip * getY(1); }

    /// Вернуть частоту вращения тягового генератора
    double getOmega() const { return getY(1); }

    /// Вернуть частоту вращения коленчатого вала в об/мин
    double getShaftFreq() const { return getY(1) * 30.0 / Physics::PI; }

    /// Вернуть фактический расход топлива
    double getFuelFlow() const { return Q_fuel; }

    float getSoundSignal(size_t pos_idx = 0) const override
    {
        return sound_states[pos_idx].createSoundSignal();
    }

    /// Температура воды горячего контура
    double getWaterTemp() const { return getY(WATER_HOT_TEMP); }

    /// Температура масла
    double getOilTemp() const { return getY(OIL_TEMP); }

    /// Температура воды холодного контура
    double getWaterColdTemp() const { return getY(WATER_COLD_TEMP); }

    /// Задать частоту вращения вентилятора горячего контура
    void setFanHotOmega(double omega) { fan_hot_omega = omega; }

    /// Задать частоту вращения вентилятора холодного контура
    void setFanColdOmega(double omega) { fan_cold_omega = omega; }

    /// Задать состояние жалюзи воды (ВЖВ)
    void setWaterBlindsState(bool open) { blinds_water_open = open; }

    /// Задать состояние жалюзи масла (ВЖМ)
    void setOilBlindsState(bool open) { blinds_oil_open = open; }

    /// Задать температуру окружающей среды
    void setTambient(double T) { T_ambient = T; }

    /// Порог перегрева воды
    double getWaterTempAlarm() const { return water_temp_alarm; }

    /// Порог перегрева масла
    double getOilTempAlarm() const { return oil_temp_alarm; }

private:

    enum
    {
        NUM_COEFFS = 10,

        /// Давление масла
        OIL_PRESSURE = 0,

        /// Угловая скорость вращения вала
        SHAFT_OMEGA = 1,

        /// Температура воды горячего контура
        WATER_HOT_TEMP = 3,

        /// Температура масла
        OIL_TEMP = 4,

        /// Температура воды холодного контура (охлаждение масла + наддув)
        WATER_COLD_TEMP = 5,
    };

    /// Условный объем маслянного русла
    double  V_oil;

    /// Расход масла от ЭМН
    double  Q_emn;

    /// Момент от стартер-генератора
    double  M_sg;

    /// Момент от возбудителя
    double  M_fg;

    /// Момент от главного генератора
    double  M_gen;

    /// Момент сопротивления на валу
    double  Mc;

    /// Передаточное число привода стартер-генератора
    double  ip;

    /// Приведенный к коленчатому валу момент инерции (условно постоянный!)
    double  J_shaft;

    /// Признак воспламенения топлива
    bool    is_fuel_ignition;

    /// Состояние вентиля МВ6
    bool    state_mv6;

    /// Состояние вентиля ВТН
    bool    state_vtn;

    /// Заданная частота вращения коленчатого вала, об/мин
    double  n_ref;

    /// Пердыдущая заданная частота
    double  n_ref_prev;

    /// Максимальный расход топлива, кг/с
    double  Q_max;

    /// Фактический расход топлива, кг/с
    double  Q_fuel;

    /// Текущий крутящий момент, развиваемый дизелем
    double  M_d;

    /// Минимальная чатота прокрутки коленчатого вала при пуске
    double  omega_min;

    /// Время прокрутки до пуска
    double  start_time;

    /// Таймер, измеряющий время прокрутки до воспламенения топлива
    Timer   *timer;

    /// Давление топлива перед ТНВД
    double  fuel_pressure;

    /// Рассогласование скорости вращения вала
    double  delta_omega;

    /// Счетчик позиций
    int     pos_count;

    /// Имя текущего проигрываемого звука
    QString soundName;

    /// Уровень топлива в баке
    double fuel_level;

    /// Тепловая мощность, кВт (рассчитывается в preStep)
    double Q_thermal = 0.0;

    // --- Термодинамика ---
    double C_water_hot = 400.0;
    double C_water_cold = 300.0;
    double C_oil = 100.0;
    double k_oil_water = 20.0;
    double k_amb_water_hot = 0.05;
    double k_amb_water_cold = 0.05;
    double k_amb_oil = 0.1;
    double k_fan_nom_hot = 6.0;
    double k_fan_nom_cold = 5.0;
    double k_blinds_nat = 0.3;
    double heat_to_water_hot = 0.75;
    double heat_to_oil = 0.15;
    double heat_to_water_cold = 0.05;
    double T_ambient = 20.0;
    double water_temp_alarm = 92.0;
    double oil_temp_alarm = 85.0;
    double fan_hot_omega = 0.0;
    double fan_cold_omega = 0.0;
    bool blinds_water_open = false;
    bool blinds_oil_open = false;
    double Q_cool_hot = 0.0;
    double Q_cool_cold = 0.0;

    enum
    {
        MIN_POS = 0,
        MAX_POS = 15
    };

    std::array<double, NUM_COEFFS>  K;

    std::array<sound_state_t, MAX_POS + 2> sound_states;

    void preStep(state_vector_t &Y, double t);

    void ode_system(const state_vector_t &Y, state_vector_t &dYdt, double t);

    void load_config(CfgReader &cfg);

    void switchDiselSound(double n_ref_prev, double n_ref);

private:

    void slotFuelIgnition();
};

#endif // DISEL_H
