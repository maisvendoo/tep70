#ifndef     ELECTRO_PNEUMATIC_VALVE_H
#define     ELECTRO_PNEUMATIC_VALVE_H

#include    "device.h"

//------------------------------------------------------------------------------
// Пневмоэлектровентиль
//
// Имеет входную камеру (накапливает давление из входного потока) и выходную
// камеру (через клапан наполняется из входной или стравливается в атмосферу).
// Состояние — нормально открытый или нормально закрытый.
//------------------------------------------------------------------------------
class ElectroPneumaticValve : public Device
{
public:

    /// Конструктор
    ElectroPneumaticValve(QObject *parent = Q_NULLPTR);

    /// Деструктор
    ~ElectroPneumaticValve();

    /// Задать поток на входе, м³/с
    void setInputFlow(double value) { Q_in = value; }

    /// Давление на входе, Па
    double getInputPressure() const { return getY(P_IN); }

    /// Задать давление на выходе от подключённого устройства, Па
    void setOutputPressure(double value) { P_device = value; }

    /// Поток на выходе к подключённому устройству, м³/с
    double getOutputFlow() const { return Q_out; }

    /// Задать напряжение на обмотке, В
    void setVoltage(double value) { U = value; }

    /// Задать режим работы: true — нормально закрытый, false — нормально открытый
    void setMode(bool normally_closed) { mode_normally_closed = normally_closed; }

    /// Состояние вентиля: true — открыт, false — закрыт
    bool isOpened() const { return is_opened; }

private:

    enum
    {
        /// Индекс давления во входной камере в векторе Y
        P_IN = 0,
        /// Индекс давления в выходной камере в векторе Y
        P_OUT = 1,
        /// Индекс тока в катушке в векторе Y
        CURRENT = 2
    };

    /// Объём входной камеры, м³
    double V_in = 0.003;

    /// Объём выходной камеры, м³
    double V_out = 0.003;

    /// Коэффициент пропускания открытого вентиля, (м³/с)/Па
    double K_flow = 1.0e-6;

    /// Коэффициент стравливания выходной полости в атмосферу при закрытом вентиле, (м³/с)/Па
    double K_atm = 5.0e-5;

    /// Коэффициент дросселя на выходе к подключённому устройству, (м³/с)/Па
    double K_choke = 1.0e-6;

    /// Номинальное напряжение катушки, В
    double U_nom = 110.0;

    /// Активное сопротивление катушки, Ом
    double R = 170.0;

    /// Постоянная времени катушки, с
    double T = 0.1;

    /// Порог выключения (от 0.0 до 1.0)
    double level_off = 0.6;

    /// Порог включения (от 0.0 до 1.0)
    double level_on = 0.7;

    /// Напряжение, подаваемое на катушку, В
    double U = 0.0;

    /// Режим работы: true — нормально закрытый, false — нормально открытый
    bool mode_normally_closed = true;

    /// Поток на входе от подключённого прибора, м³/с
    double Q_in = 0.0;

    /// Давление на выходе от подключённого устройства, Па
    double P_device = 0.0;

    /// Поток на выходе к подключённому устройству, м³/с
    double Q_out = 0.0;

    /// Текущее состояние вентиля: true — открыт, false — закрыт
    bool is_opened = false;

    /// Расход через вентиль (сохраняется между preStep и ode_system)
    double Q_valve = 0.0;

    /// Расход стравливания выхода через атмосферное отверстие (сохраняется между preStep и ode_system)
    double Q_atm = 0.0;

    /// Гистерезис тока катушки
    Hysteresis hysteresis = Hysteresis(level_off * U_nom / R,
                                        level_on * U_nom / R,
                                        false);

    void preStep(state_vector_t &Y, double t) override;

    void ode_system(const state_vector_t &Y, state_vector_t &dYdt, double t);

    void load_config(CfgReader &cfg);
};

#endif // ELECTRO_PNEUMATIC_VALVE_H