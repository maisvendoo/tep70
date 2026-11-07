#include    <edb-control-unit.h>

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
ControlUnitEDB::ControlUnitEDB(QObject *parent) : Device(parent)
{

}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
ControlUnitEDB::~ControlUnitEDB()
{

}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void ControlUnitEDB::step(double t, double dt)
{
    Device::step(t, dt);

    deps_Idt = (eps_I - eps_I_prev) / dt;
    eps_I_prev = eps_I;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void ControlUnitEDB::preStep(state_vector_t &Y, double t)
{
    is_power = Ucc > 50.0;

    if (!is_power)
    {
        u_ref = 0.0;
        Y[0] = 0.0;
        Y[1] = 0.0;
        return;
    }

    // Ошибка по скорости
    double dv = V_ref - Vkmh;

    double Ia_ref = -Ia_max;//cut(K[0] * dv, -Ia_max, 0.0);

    // Ограничение тока якоря по условиям коммутации
    const double eps_V = 0.1;

    double Ic = -Ia_max * Vc / (Vkmh + eps_V);

    Ia_ref = max(Ia_ref, Ic);

    // Ошибка по току якоря
    eps_I = Ia_ref - Ia;

    Y[0] = cut(Y[0], -integrator_lim, integrator_lim);

    // ПИД-регулятор тока
    // 1. Пропорциональная часть задает основную часть тока возбуждения
    // 2. Интегральная часть устраняет статическую ошибку по току
    // 3. Дифференциальная часть демпфирует рост задания по току, что особенно
    // важно при переходных режимах дизеля, а значит и возбудителя
    // 4. Ограничение по току возбуждения ТЭД
    double u = -K[1] * eps_I - K[2] * Y[0] - K[3] * deps_Idt - K[4] * pf(If - If_max);

    u_ref = cut(u, 0.0, 1.0);
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void ControlUnitEDB::ode_system(const state_vector_t &Y,
                                state_vector_t &dYdt,
                                double t)
{
    if (!is_power)
    {
        u_ref = 0.0;
        dYdt[0] = 0.0;
        dYdt[1] = 0.0;
        return;
    }

    dYdt[0] = eps_I;

    double T = 1.0;

    dYdt[1] = (u_ref - Y[1]) / T;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void ControlUnitEDB::load_config(CfgReader &cfg)
{
    QString secName = "Device";

    cfg.getDouble(secName, "Vmax", Vmax);
    cfg.getDouble(secName, "If_max", If_max);
    cfg.getDouble(secName, "Ia_max", Ia_max);
    cfg.getDouble(secName, "IntegratorLimit", integrator_lim);
    cfg.getDouble(secName, "Vc", Vc);

    for (size_t i = 0; i < K.size(); ++i)
    {
        QString param = QString("K%1").arg(i);
        cfg.getDouble(secName, param, K[i]);
    }
}
