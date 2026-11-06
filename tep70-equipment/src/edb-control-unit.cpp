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


}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void ControlUnitEDB::ode_system(const state_vector_t &Y,
                                state_vector_t &dYdt,
                                double t)
{

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

    for (size_t i = 0; i < K.size(); ++i)
    {
        QString param = QString("K%1").arg(i);
        cfg.getDouble(secName, param, K[i]);
    }
}
