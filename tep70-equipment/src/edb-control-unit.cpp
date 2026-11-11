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
    // Константы для защиты от деления на 0
    const double ksi_I = 0.1;

    is_power = Ucc > 50.0;

    if (!is_power)
    {
        u_ref = 0.0;
        Y[0] = 0.0;
        Y[1] = 0.0;
        V_ref = 0.0;
        is_brake_ready = false;
        return;
    }

    if (is_brake_ready)
    {
        // Контроль тормозного тока только при остановочном торможении
        // или схема собрана от крана машиниста
        if (is_stopping_brake || (!is_KM_brake))
        {
            is_brake_ready = (qAbs(Ia) > min_brake_current_level * Ia_max);
        }
    }
    else
    {
        // Контроль тормозного тока при сборе схемы
        is_brake_ready = qAbs(Ia) > min_brake_current_level * Ia_max;
    }

    V_ref = V_ref_max * (1.0 + velosity_selsin_pos) * static_cast<double>(is_KM_brake) * static_cast<double>(is_service_brake);

    // Сигнал обратной связи по скорости
    double V_fb = Vkmh + (V_test - Vkmh) * static_cast<double>(is_check_EDB);

    // Ошибка по скорости
    double dv = V_ref * (1.0 - static_cast<double>(is_check_EDB)) - V_fb;

    double Ia_ref = cut(K[0] * dv, -Ia_max, 0.0);

    // Ограничение тока якоря по условиям коммутации

    double Ic = -calcCommutationCurrentLimit(Vkmh);

    Ia_ref = max(Ia_ref, Ic);

    // Ограничение тормозного усилия от переключателя

    // Величниа, пропорциональня максимальном утормозному усилию
    double b = Ia_max + If_max;

    // Рассчитаваем ограничение по току якоря при данном токе возбуждения,
    // для ограничения тягового усилия
    double force_level = 0;

    if (is_full_brake)
    {
        if (is_service_brake)
        {
            force_level = force_level_min + force_level_step * force_limit_level;
        }
        else
        {
            force_level = force_level_max;
        }

        force_level = cut(force_level, force_level_min, force_level_max);
    }
    else
    {
        // Пердварительное торможение по сигналу внешней схемы
        force_level = cut(force_prep_level, 0.0, force_level_max);
    }

    double Ia_max_force = -pf(force_level * b - If);

    Ia_ref = max(Ia_ref, Ia_max_force);

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

    cfg.getDouble(secName, "ForcePrepLevel", force_prep_level);
    cfg.getDouble(secName, "ForceLevelMin", force_level_min);
    cfg.getDouble(secName, "ForceLevelMax", force_level_max);
    cfg.getInt(secName, "NumForceLevels", num_force_levels);

    if (num_force_levels > 1)
    {
        force_level_step = (force_level_max - force_level_min) / (num_force_levels - 1);
    }
    else
    {
        force_level_step = 0.0;
        force_level_min = force_level_max;
    }

    for (size_t i = 0; i < K.size(); ++i)
    {
        QString param = QString("K%1").arg(i);
        cfg.getDouble(secName, param, K[i]);
    }
}


//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
double ControlUnitEDB::calcCommutationCurrentLimit(double Vkmh)
{
    const double eps_V = 0.001;

    double Ia = Ia_max - (Ia_max - Ic) * (Vkmh - Vc) / (Vmax - Vc + 0.001);

    return cut(Ia, 0.0, Ia_max);
}
