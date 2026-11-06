#include    "tep70.h"

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void TEP70::stepDisel(const double& t, const double& dt)
{
    auto getWireState = [this](int km_contact) -> bool {

        bool state = (KB1_switch->getContactState(KB1_CONTACT_CAB1_K6) && km[CAB1]->isContact(km_contact)) ||
                     (KB1_switch->getContactState(KB1_CONTACT_CAB2_K14) && km[CAB2]->isContact(km_contact));

        return state;
    };

    bool is_AB2_ON = is_AB2_CAB1_on || is_AB2_CAB2_on;

    // Управление блок-контактами регулятора частоты от КМ
    bool is_MP_ON[NUM_MP] = { false };

    is_MP_ON[MP1] = getWireState(KM_24) || (is_AB2_ON && ru22->getContactState(RU22_MP1_ON));

    is_MP_ON[MP2] = getWireState(KM_37);

    is_MP_ON[MP3] = getWireState(KM_23);

    is_MP_ON[MP4] = getWireState(KM_20) ||
                    (is_AB2_ON && ru24->getContactState(RU24_FOR_MP4_ON)) ||
                    (is_AB2_ON && ru22->getContactState(RU22_MP4_ON));

    // Определяем положение регулятора
    double reg_pos = 0.0;

    for (auto i = 0; i < NUM_MP; ++i)
    {
        mp[i]->setVoltage(Ucc * static_cast<double>(is_MP_ON[i]));

        reg_pos += static_cast<int>(mp[i]->getContactState(MP_STATE)) * mp_weight[i];

        mp[i]->step(t, dt);
    }

    disel->setQ_emn(electro_oil_pump->getOilFlow());
    disel->setStarterTorque(starter_generator->getTorque());
    disel->setFGTorque(field_gen->getTorque());
    disel->setGenTorque(trac_gen->getTorque());
    disel->setFuelPressure(electro_fuel_pump->getFuelPressure());
    disel->setMV6state(mv6->getContactState(0));
    disel->setVTNstate(vtn->getContactState(0));
    //disel->setRefFreq(max(km[CAB1]->getRefFreq(), km[CAB2]->getRefFreq())); // ??????
    disel->setRegulatorPosition(reg_pos);
    disel->setFuelLevel(fuel_tank->getFuelLevel());
    disel->step(t, dt);

    double I_gen_starter = Icc + motor_compressor->getPowerCurrent();

    starter_generator->setAncorVoltage( battery->getVoltage() *  static_cast<double>(kontaktor_starter->getContactState(0)));
    starter_generator->setFieldVoltage(voltage_regulator->getFieldVoltage() * static_cast<double>(krn->getContactState(0)));
    starter_generator->setLoadCurrent(I_gen_starter);
    starter_generator->setOmega(disel->getStarterOmega());
    starter_generator->setMotorMode(krn->getContactState(2));
    starter_generator->step(t, dt);

    voltage_regulator->setBatteryVoltage(battery->getVoltage());
    voltage_regulator->setRefVoltage(110.0);
    voltage_regulator->setVoltage(starter_generator->getVoltage());
    voltage_regulator->step(t, dt);
}
