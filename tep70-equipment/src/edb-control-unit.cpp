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
void ControlUnitEDB::preStep(state_vector_t &Y, double t)
{
    is_power = Ucc > 50.0;

    if (!is_power)
    {
        If_ref = 0.0;
        return;
    }

    double dv = V_ref - Vkmh;

    double Ia_ref = cut(K[0] * dv, -Ia_max, 0.0);

    eps_I = (Ia_ref - Ia) / Ia_max;

    Y[0] = cut(Y[0], -integrator_lim, integrator_lim);

    If_ref = cut(-K[1] * eps_I - K[2] * Y[0], 0.0, 1.0) * If_max;
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
        If_ref = 0.0;
        dYdt[0] = 0.0;
        return;
    }

    dYdt[0] = eps_I;
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

    for (size_t i = 0; i < K.size(); ++i)
    {
        QString param = QString("K%1").arg(i);
        cfg.getDouble(secName, param, K[i]);
    }
}
