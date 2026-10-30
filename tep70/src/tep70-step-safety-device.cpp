#include    "tep70.h"

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void TEP70::stepSafetyDevices(const double& t, const double& dt)
{
    // Приёмные катушки АЛСН
    coil_ALSN_fwd->step(t, dt);
    coil_ALSN_bwd->step(t, dt);

    // Дешифратор АЛСН
    alsn_decoder[CAB1]->setCoilSignal(coil_ALSN_fwd->getCode());
    alsn_decoder[CAB1]->step(t, dt);

    alsn_decoder[CAB2]->setCoilSignal(coil_ALSN_bwd->getCode());
    alsn_decoder[CAB2]->step(t, dt);

    // Скоростемер
    speed_meter[CAB1]->setOmega(wheel_omega[0]);
    speed_meter[CAB1]->step(t, dt);

    speed_meter[CAB2]->setOmega(wheel_omega[5]);
    speed_meter[CAB2]->step(t, dt);

    // Цепь питания ЭПК

    // Общее питание ЭПК и приборов безопасности
    bool is_EPKs_common_power = azv_epk_power_plus.getState() && azv_epk_power_minus.getState();

    bool is_EPK_power[CABS_NUM] = {false, false};

    // Цепи питания ЭПК по кабинам
    is_EPK_power[CAB1] = is_EPKs_common_power &&
                         KB2_switch->getContactState(KB2_CONTACT_CAB1_K7);

    is_EPK_power[CAB2] = is_EPKs_common_power &&
                         KB2_switch->getContactState(KB2_CONTACT_CAB2_K15);

    // Цепи питания УКБМ
    bool is_ukbm_power[CABS_NUM] = {false, false};

    is_ukbm_power[CAB1] = is_EPKs_common_power &&
                          KB2_switch->getContactState(KB2_CONTACT_CAB1_K1) &&
                          KB2_switch->getContactState(KB2_CONTACT_CAB1_K3);

    is_ukbm_power[CAB2] = is_EPKs_common_power &&
                          KB2_switch->getContactState(KB2_CONTACT_CAB2_K9) &&
                          KB2_switch->getContactState(KB2_CONTACT_CAB2_K11);

    for (size_t cab_idx : {CAB1, CAB2})
    {
        // УКБМ
        safety_device[cab_idx]->setAlsnCode(alsn_decoder[cab_idx]->getCode());
        safety_device[cab_idx]->setRBstate(rb[cab_idx][RB].getState());
        safety_device[cab_idx]->setRBSstate(rb[cab_idx][RBS].getState());
        safety_device[cab_idx]->setKeyEPK(epk[cab_idx]->isKeyOn() && is_ukbm_power[cab_idx]);
        safety_device[cab_idx]->setVelocity(speed_meter[cab_idx]->getVelocity());
        safety_device[cab_idx]->setShuntingModeState(shunting_mode_switcher[cab_idx].getState());
        safety_device[cab_idx]->step(t, dt);

        // Электропневматический клапан автостопа
        epk[cab_idx]->setFLpressure(main_reservoir->getPressure());
        epk[cab_idx]->setBPpressure(brakepipe->getPressure());
        epk[cab_idx]->setPowered(safety_device[cab_idx]->getEPKstate() && is_EPK_power[cab_idx]);
        epk[cab_idx]->step(t, dt);
    }
}
