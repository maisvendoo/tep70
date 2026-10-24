#include    <tep70.h>
#include    <tep70-controls.h>

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void TEP70::stepLighting(const double &t, const double &dt)
{
    // Уровень напряжения питания
    float power_level = Ucc / 110.0f;

    for (auto cab_idx : {CAB1, CAB2})
    {
        // Включен соответствующий автомат и тумблер
        bool is_cab_light = azv_cabine_light[cab_idx].getState() &&
                            tumbler_cabine_light[cab_idx].getState();

        cab_light_signal[cab_idx] = static_cast<float>(is_cab_light) * power_level;

        // Освещение приборов
        bool is_gauges_light = azv_panel_light[cab_idx].getState();

        gauges_light_signal[cab_idx] = static_cast<float>(is_gauges_light) * control_inputs[cab_idx][CTRL_GAUGES_LIGHT].value  * power_level;

        // Буферные фонари левые
        bool is_buf_light_left_white = azv_buffer_lights.getState() && tumbler_bufferliht_L[cab_idx].isSwitched(2);
        bool is_buf_light_left_red = azv_buffer_lights.getState() && tumbler_bufferliht_L[cab_idx].isSwitched(0);

        buf_lights_left[cab_idx][BUF_WHITE] = static_cast<float>(is_buf_light_left_white) * power_level;
        buf_lights_left[cab_idx][BUF_RED] = static_cast<float>(is_buf_light_left_red) * power_level;

        // Буферные фонари правые
        bool is_buf_light_right_white = azv_buffer_lights.getState() && tumbler_bufferliht_R[cab_idx].isSwitched(2);
        bool is_buf_light_right_red = azv_buffer_lights.getState() && tumbler_bufferliht_R[cab_idx].isSwitched(0);

        buf_lights_right[cab_idx][BUF_WHITE] = static_cast<float>(is_buf_light_right_white) * power_level;
        buf_lights_right[cab_idx][BUF_RED] = static_cast<float>(is_buf_light_right_red) * power_level;

        // Прожекторы
        float intensity = (1.0f + static_cast<float>(azv_spotlight_high[cab_idx].getState())) * static_cast<float>(azv_spotlight_low[cab_idx].getState());

        spotlight_signal[cab_idx] = intensity * power_level / 2.0f;
    }
}
