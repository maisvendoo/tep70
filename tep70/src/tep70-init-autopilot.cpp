#include    <tep70.h>

#include    <core/load_module.h>

#include    <QDir>

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void TEP70::initAutopilot(const QString& modules_dir,
                          const QString& custom_cfg_dir)
{
    // Модули автоведения
    for (auto cab_idx : {CAB1, CAB2})
    {
        Autopilot* autopilot = LOAD_MODULE(Autopilot,
            modules_dir + QDir::separator() +
            custom_modules_dir + QDir::separator() +
            autopilot_module_name);

        if (autopilot != nullptr)
        {
            autopilot->setVehicleIndex(model_idx);
            autopilot->read_config(autopilot_config_name, custom_cfg_dir);
            autopilot->initAutoBrakeControl(autopilot_config_name, custom_cfg_dir);
            autopilot_switcher[cab_idx].setKeyModifierOn(MODIFIER_OnlyAlt);
            autopilot_switcher[cab_idx].setKeySymbolOn(KEY_F);
            autopilot_switcher[cab_idx].setKeyModifierOff(MODIFIER_OnlyAlt);
            autopilot_switcher[cab_idx].setKeySymbolOff(KEY_F);
            autopilot_switcher[cab_idx].setControl(&pressed_keys);

            connect(autopilot, &Autopilot::sigInitTrainParams, this, &TEP70::slotInitTrainForAutopilot);

            this->autopilot.push_back(autopilot);

            auto_feedback[cab_idx] = new tep70_feedback_t();
        }
    }
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void TEP70::slotInitTrainForAutopilot()
{
    double train_len = 0;
    double train_mass = 0;

    emit sigGetTrainParams(train_idx, train_len, train_mass);

    for (auto cab_idx : {CAB1, CAB2})
    {
        autopilot[cab_idx]->setTrainLength(train_len);
        autopilot[cab_idx]->setTrainMass(train_mass);
    }

    // Кое-какие другие действия при активации автоведения
    if (km[CAB1]->isReversHandle())
    {
        prepareCabineForAutopilot(CAB1, CAB2);
    }

    if (km[CAB2]->isReversHandle())
    {
        prepareCabineForAutopilot(CAB2, CAB1);
    }
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void TEP70::prepareCabineForAutopilot(int my_cab_idx, int other_cab_idx)
{
    // В нашей кабине

    // включаем ЭПТ
    if (!epb_control->stateReleaseLamp())
    {
        azv_ept_power[my_cab_idx].set();
        azv_ept_on[my_cab_idx].set();
    }

    tumbler_bufferliht_L[my_cab_idx].setPosition(2);
    tumbler_bufferliht_R[my_cab_idx].setPosition(2);
    azv_spotlight_high[my_cab_idx].set();

    // В другой кабине
    tumbler_bufferliht_L[other_cab_idx].setPosition(2);
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void TEP70::OnAutopilot()
{
    Vehicle::OnAutopilot();

    // Делаем автозапуск
    if (km[CAB1]->isReversHandle())
    {
        initAutostartProgram(CAB1);
        //autopilot_switcher[CAB1].set();
    }

    if (km[CAB2]->isReversHandle())
    {
        initAutostartProgram(CAB2);
        //autopilot_switcher[CAB2].set();
    }

    autoStartTimer->start();
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void TEP70::OffAutopilot()
{
    for (auto cab_idx : {CAB1, CAB2})
    {
        autopilot_switcher[cab_idx].reset();
    }
}
