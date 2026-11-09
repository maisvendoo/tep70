#include    "electro-pneumatic-valve.h"


//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
ElectroPneumaticValve::ElectroPneumaticValve(QObject *parent) : Device(parent)
{

}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
ElectroPneumaticValve::~ElectroPneumaticValve()
{

}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void ElectroPneumaticValve::preStep(state_vector_t &Y, double t)
{
    Q_UNUSED(t)

    // Состояние реле по току в катушке
    double I = Y[CURRENT];
    hysteresis.setValue(I);
    bool relay_energized = hysteresis.getState();

    // Положение вентиля в зависимости от режима
    if (mode_normally_closed)
    {
        is_opened = relay_energized;
    }
    else
    {
        is_opened = !relay_energized;
    }

    double P_out = Y[P_OUT];
    double P_in  = Y[P_IN];    
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void ElectroPneumaticValve::ode_system(const state_vector_t &Y,
                                        state_vector_t &dYdt, double t)
{
    Q_UNUSED(t)

    // Расход через открытый вентиль
    Q_valve = 0.0;

    if (is_opened)
    {
        Q_valve = K_flow * (Y[P_IN] - Y[P_OUT]);
    }

    // Стравливание выходной камеры в атмосферу при закрытом вентиле
    Q_atm = 0.0;

    if (!is_opened)
    {
        Q_atm = K_atm * Y[P_OUT];
    }

    // Расход к подключённому устройству
    Q_out = K_choke * (Y[P_OUT] - P_device);

    // Баланс потоков для входной камеры
    dYdt[P_IN] = (Q_in - Q_valve) / V_in;

    // Баланс потоков для выходной камеры
    dYdt[P_OUT] = (Q_valve - Q_out - Q_atm) / V_out;

    // Ток в катушке реле
    dYdt[CURRENT] = (U / R - Y[CURRENT]) / T;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void ElectroPneumaticValve::load_config(CfgReader &cfg)
{
    QString secName = "Device";

    cfg.getDouble(secName, "V_in", V_in);
    cfg.getDouble(secName, "V_out", V_out);
    cfg.getDouble(secName, "K_flow", K_flow);
    cfg.getDouble(secName, "K_atm", K_atm);
    cfg.getDouble(secName, "K_choke", K_choke);
    cfg.getDouble(secName, "U_nom", U_nom);
    cfg.getDouble(secName, "R", R);
    cfg.getDouble(secName, "T", T);
    cfg.getDouble(secName, "level_off", level_off);
    cfg.getDouble(secName, "level_on", level_on);

    int mode = 1;
    cfg.getInt(secName, "Mode", mode);
    mode_normally_closed = (mode == 1);

    // Пороги срабатывания по току: level_off, level_on
    hysteresis = Hysteresis(level_off * U_nom / R,
                             level_on * U_nom / R,
                             false);
}
