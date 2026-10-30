#include    <tep70.h>

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void TEP70::stepGaugesPower(double t, double dt)
{
    for (auto cab_idx : {CAB1, CAB2})
    {
        float is_gauges_power = azv_gauges_power.getState() &&
                                tumbler_gauges_power[cab_idx].getState() && (Ucc > 1.0);

        water_temp_gauge_power[cab_idx] = static_cast<float>(is_gauges_power);
        oil_temp_gauge_power[cab_idx] = static_cast<float>(is_gauges_power);
        fuel_press_gauge_power[cab_idx] = static_cast<float>(is_gauges_power);
        oil_press_gauge_power[cab_idx] = static_cast<float>(is_gauges_power);
    }
}
