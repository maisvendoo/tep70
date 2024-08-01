#include    "tep70bs.h"

void TEP70BS::stepMSUTsignals(double t, double dt)
{
    Q_UNUSED(t)
    Q_UNUSED(dt)

    msut_input.velocity = velocity;
    msut_input.is_KP1_KP6_on = is_KP1_KP6_on;
    msut_input.bc_pressure = brake_mech[TROLLEY_FWD]->getBCpressure();
    msut_input.button_start_state = button_start_disel.getState();
    msut_input.button_stop_state = false;

    msut_input.is_KMN_on = kontaktor_oil_pump->getContactState(1);
    msut_input.is_KD_on = kontaktor_starter->getContactState(1);

    msut->setInputData(msut_input);

    msut_output = msut->getOutputData();

    analogSignal[MSUT_REVERSOR] = reversor->getSatate();
    analogSignal[MSUT_SPEED] = velocity * 3.6;

    analogSignal[MSUT_VU2_I] = 0;
    analogSignal[MSUT_VU2_U] = 0;

    analogSignal[MSUT_VU1_I] = I_gen / 1000.0;
    analogSignal[MSUT_VU1_U] = trac_gen->getVoltage() / 1000.0;
    analogSignal[MSUT_VU1_I_TED] = motor[0]->getAncorCurrent() / 1000.0;

    analogSignal[MSUT_POSITION] = km->getPositionNumber();

    double power_kW = I_gen * trac_gen->getVoltage() / 1000.0;
    analogSignal[MSUT_POWER] = power_kW;

    analogSignal[MSUT_ACCELLERATION] = msut_output.acceleration;

    double traction = 0;

    for (size_t i = 1; i < Q_a.size(); ++i)
    {
        traction += 2 * Q_a[i] / wheel_diameter[i - 1];
    }

    analogSignal[MSUT_ET_T] = traction / Physics::g / 1000.0;

    analogSignal[MSUT_MODE] = msut_output.mode;

    analogSignal[MSUT_NOMER_KADR_DISP] = msut_output.screen_num;

    analogSignal[MSUT_T_OIL] = 75;
    analogSignal[MSUT_T_WHATER] = 95;

    analogSignal[MSUT_P_OIL] = disel->getOilPressure() * Physics::g;
    analogSignal[MSUT_P_FUEL] = electro_fuel_pump->getFuelPressure() * Physics::g;
    analogSignal[MSUT_I_AB] = hs_p(battery->getChargeCurrent());
    analogSignal[MSUT_U_CHAIN] = Ucc;

    analogSignal[MSUT_TIMER_PROKACHKA] = msut_output.oil_pump_timer;
    analogSignal[MSUT_TIMER_PROKRUTKA] = msut_output.starter_timer;
    analogSignal[MSUT_TIMER_OSTANOV] = msut_output.stop_timer;
}
