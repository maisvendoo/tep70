#include    "tep70bs.h"


void TEP70BS::signalsOutput(const simulator_time_t& t, const double& dt)
{
    (void) t;
    (void) dt;

    analogSignal[STRELKA_REOSTATE_CURRENT] = 0.0;

    analogSignal[STRELKA_WATER_TEMP] = 0.0;

    // Состояние локомотивного светофора
    analogSignal[LS_G] = static_cast<float>(alsn_decoder[CAB1]->getCode() == ALSN::GREEN);
    analogSignal[LS_Y] = static_cast<float>(alsn_decoder[CAB1]->getCode() == ALSN::YELLOW);
    analogSignal[LS_YR] = static_cast<float>(alsn_decoder[CAB1]->getCode() == ALSN::RED_YELLOW);
    analogSignal[LS_R] = 0.0f;
    analogSignal[LS_W] = static_cast<float>(alsn_decoder[CAB1]->getCode() == ALSN::NO_CODE);

    analogSignal[KM_SHTURVAL] = km[CAB1]->getMainShaftPos();
    analogSignal[KM_REVERSOR] = tumbler_revers[CAB1].getPosition() - 1;

    analogSignal[BUTTON_DISEL_START] = static_cast<float>(button_disel_start[CAB1].getState());
    analogSignal[BUTTON_BRAKE_RELEASE] = static_cast<float>(button_brake_release[CAB1].getState());
    analogSignal[BUTTON_SVISTOK] = static_cast<float>(button_svistok[CAB1].getState());
    analogSignal[BUTTON_TIFON] = static_cast<float>(button_tifon[CAB1].getState());

    analogSignal[AZV_COMMON_CONTROL] = static_cast<float>(azv_common_control[CAB1].getState());
    analogSignal[AZV_UPR_TEPL] = static_cast<float>(azv_upr_tepl[CAB1].getState());
    analogSignal[AZV_FUEL_PUMP] = static_cast<float>(azv_fuel_pump[CAB1].getState());
    analogSignal[AZV_EDT_ON] = static_cast<float>(azv_edt_on[CAB1].getState());
    analogSignal[AZV_EDT_POWER] = static_cast<float>(azv_edt_power[CAB1].getState());
    analogSignal[AZV_EPT_ON] = static_cast<float>(azv_ept_on[CAB1].getState());

    analogSignal[TUMBLER_VOLTMETER] = static_cast<float>(tumbler_voltage[CAB1].getState());
    analogSignal[TUMBLER_DISEL_STOP] = static_cast<float>(tumbler_disel_stop[CAB1].getState());

    analogSignal[TUMBLER_FIELD_WEAK1] = static_cast<float>(tumbler_field_weak1[CAB1].getHandlePosition());
    analogSignal[TUMBLER_FIELD_WEAK2] = static_cast<float>(tumbler_field_weak2[CAB1].getHandlePosition());
    analogSignal[TUMBLER_WATER_ZALUZI] = static_cast<float>(tumbler_water_zaluzi[CAB1].getHandlePosition());
    analogSignal[TUMBLER_OIL_ZALUZI] = static_cast<float>(tumbler_oil_zaluzi[CAB1].getHandlePosition());

    analogSignal[STRELKA_BAT_CURRENT] = static_cast<float>(battery->getChargeCurrent() / 150.0);

    double U_bat;
    U_bat = tumbler_voltage[CAB1].getState() ? epb_converter->getOutputVoltage() : Ucc;
    analogSignal[STRELKA_BAT_VOLTAGE] = static_cast<float>(U_bat / 150.0);

    analogSignal[STRELKA_FUEL_PRESS] = static_cast<float>(electro_fuel_pump->getFuelPressure() * Physics::g / 15.0);
    analogSignal[STRELKA_OIL_PRESS] = static_cast<float>(disel->getOilPressure() * Physics::g / 15.0);

    analogSignal[SIGLIGHT_OIL_PRESS] = getLampState(hs_p(0.1 - disel->getOilPressure()));
    analogSignal[SIGLIGHT_ZB] = getLampState(hs_p(100.0 - starter_generator->getVoltage()));

    analogSignal[STRELKA_PM] = static_cast<float>(main_reservoir->getPressure() / 1.6);

    analogSignal[RUK_367] = static_cast<float>(brake_lock[CAB1]->getLockHandlePosition());
    analogSignal[COMB_KRAN] = static_cast<float>(brake_lock[CAB1]->getCombineCraneHandlePosition());

    analogSignal[STRELKA_TM] = static_cast<float>(brakepipe->getPressure() / 1.6);
    analogSignal[KRAN_395_RUK] = static_cast<float>(brake_crane[CAB1]->getHandlePosition());

    analogSignal[STRELKA_TC1] = static_cast<float>(brake_mech[TROLLEY_FWD]->getBCpressure() / 1.0);
    analogSignal[STRELKA_TC2] = static_cast<float>(brake_mech[TROLLEY_BWD]->getBCpressure() / 1.0);

    analogSignal[STRELKA_UR] = static_cast<float>(brake_crane[CAB1]->getERpressure() / 1.0);

    analogSignal[KRAN_254_POD] = static_cast<float>(loco_crane[CAB1]->getHandleShift());
    analogSignal[KRAN_254_RUK] = static_cast<float>(loco_crane[CAB1]->getHandlePosition());

    analogSignal[KLUCH_EPK] = static_cast<float>(epk[CAB1]->isKeyOn());
    analogSignal[RB1] = static_cast<float>(rb[CAB1][RB].getState());

    analogSignal[SIGLIGHT_EPT_O] = static_cast<float>(epb_control->stateReleaseLamp());
    analogSignal[SIGLIGHT_EPT_P] = static_cast<float>(epb_control->stateHoldLamp());
    analogSignal[SIGLIGHT_EPT_T] = static_cast<float>(epb_control->stateBrakeLamp());

    analogSignal[STRELKA_GEN_CURRENT] = static_cast<float>(I_gen / 10000.0);
    analogSignal[STRELKA_GEN_VOLTAGE] = static_cast<float>(trac_gen->getVoltage() / 1000.0);

    analogSignal[KLUB_ALARM] = 0.0f;

    analogSignal[WHEEL_1] = static_cast<float>(wheel_rotation_angle[0] / 2.0 / Physics::PI);
    analogSignal[WHEEL_2] = static_cast<float>(wheel_rotation_angle[1] / 2.0 / Physics::PI);
    analogSignal[WHEEL_3] = static_cast<float>(wheel_rotation_angle[2] / 2.0 / Physics::PI);
    analogSignal[WHEEL_4] = static_cast<float>(wheel_rotation_angle[3] / 2.0 / Physics::PI);
    analogSignal[WHEEL_5] = static_cast<float>(wheel_rotation_angle[4] / 2.0 / Physics::PI);
    analogSignal[WHEEL_6] = static_cast<float>(wheel_rotation_angle[5] / 2.0 / Physics::PI);
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
float TEP70BS::getLampState(double signal)
{
    bool state = azv_common_control[CAB1].getState() && static_cast<bool>(signal);

    return static_cast<float>(state);
}
