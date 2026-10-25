#include    "disel.h"

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
Disel::Disel(QObject *parent) : Device(parent)
  , V_oil(1.5)
  , Q_emn(0.0)
  , M_sg(0.0)
  , M_fg(0.0)
  , M_gen(0.0)
  , Mc(0.0)
  , ip(3.3)
  , J_shaft(1.0)
  , is_fuel_ignition(false)
  , state_mv6(false)
  , state_vtn(false)
  , n_ref(350.0)
  , n_ref_prev(n_ref)
  , Q_max(0.166)
  , Q_fuel(0.0)
  , M_d(0.0)
  , omega_min(19.9)
  , start_time(3.0)
  , timer(new Timer)
  , fuel_pressure(0.0)
  , delta_omega(0.0)
  , pos_count(0)
  , soundName("pos0")
  , fuel_level(0.0)
{
    std::fill(K.begin(), K.end(), 0.0);

    K[0] = 15.0;
    K[1] = 0.0774;
    K[2] = K[3] = 1.7e5;
    K[4] = 0.01;

    connect(timer, &Timer::process, this, &Disel::slotFuelIgnition);
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
Disel::~Disel()
{

}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void Disel::step(double t, double dt)
{
    timer->step(t, dt);
    Device::step(t, dt);
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void Disel::setQ_emn(double Q_emn)
{
    this->Q_emn = Q_emn;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void Disel::setStarterTorque(double M_sg)
{
    this->M_sg = M_sg;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void Disel::preStep(state_vector_t &Y, double t)
{
    Q_UNUSED(t)

    if (!is_fuel_ignition)
    {
        if (static_cast<bool>(hs_p(Y[1] - omega_min)))
        {
            timer->start();
        }

        if (Y[1] < 1.0)
            sound_states[pos_count].play(false);
    }
    else
    {
        is_fuel_ignition = state_mv6 && (fuel_level >= 0.01);
    }

    delta_omega = n_ref * Physics::PI / 30.0 - Y[1];

    Y[2] = cut(Y[2], 0.0, 1.0);

    double s1 = (K[4] * delta_omega + K[5] * Y[2]) * static_cast<double>(state_mv6);

    double u = cut(s1, 0.0, 1.0);

    double Q1 = u * static_cast<double>(state_vtn) * Q_max;

    double Q2 = u * Q_max;

    Q_fuel = K[6] * (Q1 + Q2);

    double M1 = K[2] * Q1;

    double M2 = K[3] * Q2;

    M_d = (M1 + M2) * static_cast<double>(is_fuel_ignition);

    switchDiselSound(n_ref_prev, n_ref);

    sound_states[pos_count].pitch = static_cast<float>(getShaftFreq() / n_ref);
    sound_states[pos_count].volume = static_cast<float>(Y[1] / 36.7);

    // --- Термодинамика ---
    if (Y[WATER_HOT_TEMP] < 1.0 && Y[OIL_TEMP] < 1.0)
    {
        Y[WATER_HOT_TEMP] = T_ambient;
        Y[OIL_TEMP] = T_ambient;
        Y[WATER_COLD_TEMP] = T_ambient;
    }

    double P_mech = M_d * Y[1] / 1000.0;
    Q_thermal = P_mech * 0.65;

    // Теплоотвод (рассчитывается в ode_system, где доступен Y)
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void Disel::ode_system(const state_vector_t &Y, state_vector_t &dYdt, double t)
{
    Q_UNUSED(t)

    // Давление масла в системе смазки
    dYdt[0] = (Q_emn + K[1] * Y[1] - K[0] * Y[0]) / V_oil;

    double Mr = cut(Mc * Y[1] / 20.0, -Mc, Mc);

    // Частота вращения коленчатого вала
    dYdt[1] = (M_d + ip * M_sg - ip * M_fg - M_gen - Mr) / J_shaft;

    dYdt[2] = delta_omega;

    // --- Термодинамика ---

    // Теплообмен масло ↔ холодная вода
    double Q_oil_to_water_cold = k_oil_water * (Y[OIL_TEMP] - Y[WATER_COLD_TEMP]);

    // Теплоотвод радиатора горячего контура
    double fan_speed_ratio_hot = (fan_hot_omega > 0.5) ? cut(fan_hot_omega / 70.0, 0.0, 1.0) : 0.0;
    Q_cool_hot = 0.0;
    if (blinds_water_open)
    {
        if (fan_hot_omega > 0.5)
            Q_cool_hot = k_fan_nom_hot * (Y[WATER_HOT_TEMP] - T_ambient) * fan_speed_ratio_hot;
        else
            Q_cool_hot = k_blinds_nat * (Y[WATER_HOT_TEMP] - T_ambient);
    }

    dYdt[WATER_HOT_TEMP] = (Q_thermal * heat_to_water_hot * 1000.0
                            - k_oil_water * (Y[WATER_HOT_TEMP] - Y[OIL_TEMP]) * 1000.0
                            - Q_cool_hot * 1000.0
                            - k_amb_water_hot * (Y[WATER_HOT_TEMP] - T_ambient) * 1000.0)
                           / (C_water_hot * 1000.0);

    // Температура масла
    dYdt[OIL_TEMP] = (Q_thermal * heat_to_oil * 1000.0
                      - k_oil_water * (Y[OIL_TEMP] - Y[WATER_COLD_TEMP]) * 1000.0
                      - k_amb_oil * (Y[OIL_TEMP] - T_ambient) * 1000.0)
                     / (C_oil * 1000.0);

    // Теплоотвод радиатора холодного контура
    double fan_speed_ratio_cold = (fan_cold_omega > 0.5) ? cut(fan_cold_omega / 70.0, 0.0, 1.0) : 0.0;
    Q_cool_cold = 0.0;
    if (blinds_oil_open)
    {
        if (fan_cold_omega > 0.5)
            Q_cool_cold = k_fan_nom_cold * (Y[WATER_COLD_TEMP] - T_ambient) * fan_speed_ratio_cold;
        else
            Q_cool_cold = k_blinds_nat * (Y[WATER_COLD_TEMP] - T_ambient);
    }

    dYdt[WATER_COLD_TEMP] = (Q_oil_to_water_cold * 1000.0
                             + Q_thermal * heat_to_water_cold * 1000.0
                             - Q_cool_cold * 1000.0
                             - k_amb_water_cold * (Y[WATER_COLD_TEMP] - T_ambient) * 1000.0)
                            / (C_water_cold * 1000.0);
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void Disel::load_config(CfgReader &cfg)
{
    QString secName = "Device";

    cfg.getDouble(secName, "V_oil", V_oil);
    cfg.getDouble(secName, "Mc", Mc);
    cfg.getDouble(secName, "J_shaft", J_shaft);
    cfg.getDouble(secName, "ip", ip);
    cfg.getDouble(secName, "Qmax", Q_max);
    cfg.getDouble(secName, "omega_min", omega_min);
    cfg.getDouble(secName, "start_time", start_time);

    timer->firstProcess(false);
    timer->setTimeout(start_time);

    for (size_t i = 0; i < K.size(); ++i)
    {
        QString param = QString("K%1").arg(i);
        cfg.getDouble(secName, param, K[i]);
    }

    secName = "Thermodynamics";
    cfg.getDouble(secName, "C_water_hot", C_water_hot);
    cfg.getDouble(secName, "C_water_cold", C_water_cold);
    cfg.getDouble(secName, "C_oil", C_oil);
    cfg.getDouble(secName, "k_oil_water", k_oil_water);
    cfg.getDouble(secName, "k_amb_water_hot", k_amb_water_hot);
    cfg.getDouble(secName, "k_amb_water_cold", k_amb_water_cold);
    cfg.getDouble(secName, "k_amb_oil", k_amb_oil);
    cfg.getDouble(secName, "k_fan_nom_hot", k_fan_nom_hot);
    cfg.getDouble(secName, "k_fan_nom_cold", k_fan_nom_cold);
    cfg.getDouble(secName, "k_blinds_nat", k_blinds_nat);
    cfg.getDouble(secName, "heat_to_water_hot", heat_to_water_hot);
    cfg.getDouble(secName, "heat_to_oil", heat_to_oil);
    cfg.getDouble(secName, "heat_to_water_cold", heat_to_water_cold);
    cfg.getDouble(secName, "T_ambient", T_ambient);
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void Disel::switchDiselSound(double n_ref_prev, double n_ref)
{
    int n0 = static_cast<int>(n_ref_prev);
    int n1 = static_cast<int>(n_ref);

    if (n0 != n1)
    {
        sound_states[pos_count].play(false);

        if (n1 > n0)
        {
            pos_count++;
        }
        else
        {
            pos_count--;
        }

        pos_count = cut(pos_count, static_cast<int>(MIN_POS), static_cast<int>(MAX_POS));

        sound_states[pos_count].play(true);
    }
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void Disel::slotFuelIgnition()
{
    is_fuel_ignition = state_mv6 && static_cast<bool>(hs_p(fuel_pressure - 0.1));
    sound_states[pos_count].play(true);
    timer->stop();
}
