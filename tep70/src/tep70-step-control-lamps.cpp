#include    <tep70.h>

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
float TEP70::getLampState(int cab_idx, double signal)
{
    // Состояние блокировки
    bool kb_sw_state[CABS_NUM] = {false, false};
    kb_sw_state[CAB1] = KB1_switch->getContactState(KB1_CONTACT_CAB1_K8);
    kb_sw_state[CAB2] = KB1_switch->getContactState(KB1_CONTACT_CAB2_K16);

    bool state = kb_sw_state[cab_idx] &&
                 azv_common_control[cab_idx].getState() &&
                 static_cast<bool>(signal);

    return static_cast<float>(state) * power_level;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void TEP70::stepControlLamps(const double &t, const double &dt)
{
    for (auto cab_idx : {CAB1, CAB2})
    {
        control_lamps[cab_idx][LAMP_BATTARY_CHARGE] = getLampState(cab_idx, hs_p(100.0 - starter_generator->getVoltage()));
        control_lamps[cab_idx][LAMP_OIL_PRESSURE] = getLampState(cab_idx, hs_p(0.1 - disel->getOilPressure()));

        control_lamps[cab_idx][LAMP_WATER_OIL_TEMP] = getLampState(cab_idx, static_cast<double>(ru3->getContactState(RU3_TEMP_LAMP)));

        control_lamps[cab_idx][LAMP_EDT_BLINDS_OPEN_1] = 0.0f;
        control_lamps[cab_idx][LAMP_EDT_BLINDS_OPEN_2] = 0.0f;

        // Питание лампы ЛС2 "Сброс нагрузки"
        bool is_reset_power_lamp_ON = (is_Revers_Forward || is_Revers_Backward) &&
                                      kvv->getContactState(KVV_CONTACT_POWER_RESET) &&
                                      kt->getContactState(KT_RESET_POWER);

        control_lamps[cab_idx][LAMP_POWER_RESET] = getLampState(cab_idx, static_cast<double>(is_reset_power_lamp_ON));

        bool is_reserved_fuel_pump_lamp_ON = (is_AB4_CAB1_on || is_AB4_CAB2_on) &&
                                             tumbler_disel_stop[CAB1].getState() &&
                                             tumbler_disel_stop[CAB2].getState() && reserved_fuel_pump.getState();

        control_lamps[cab_idx][LAMP_RESERVED_FUEL_PUMP] = getLampState(cab_idx, static_cast<double>(is_reserved_fuel_pump_lamp_ON));

        bool is_edb_blinds_open = ru37->getContactState(RU37_EDB_BLINGS_LAMP);

        // Лампа ЛС6 "Жалюзи ЭДТ открыты"
        control_lamps[cab_idx][LAMP_EDB_BLINDS_OPEN] = getLampState(cab_idx, static_cast<double>(is_edb_blinds_open));

        bool is_edb_ready = ru28->getContactState(RU28_FOR_LS5_ON);

        // Лампа ЛС5 "Тормоз электрический"
        control_lamps[cab_idx][LAMP_EDB_READY] = getLampState(cab_idx, static_cast<double>(is_edb_ready));
    }
}
