#include    <key-symbols.h>
#include    <io-controller.h>
#include    <tep70-io-controller.h>
#include    <tep70-controls.h>
#include    <core/get_module.h>
#include    <switcher-handler.h>

// Константы положений реверсора и штурвала КМ-2202
// (соответствуют одноимённым enum в km-2202.h)
enum
{
    MS_ZERO = 0,
    RS_FORWARD = 1,
    RS_ZERO = 0,
    RS_BACKWARD = -1
};

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
TEP70IOController::TEP70IOController() : IOController(nullptr)
{

}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void TEP70IOController::init()
{
    IOController::init();

    for (auto cab_idx : {CAB1, CAB2})
    {
        ControlHandler* handler;

        handler = control_handlers[cab_idx].getByKey1(CTRL_REVERS_POSITION).value();
        revers_handle[cab_idx] = handler;

        handler = control_handlers[cab_idx].getByKey1(CTRL_KM_MAIN_POSITION).value();
        main_handle[cab_idx] = handler;

        handler = control_handlers[cab_idx].getByKey1(CTRL_CABINE_1).value();
        cabine1_switch_key[cab_idx] = handler;

        handler = control_handlers[cab_idx].getByKey1(CTRL_CABINE_2).value();
        cabine2_switch_key[cab_idx] = handler;

        handler = control_handlers[cab_idx].getByKey1(CTRL_AZV_FUEL_PUMP).value();
        azv_fuel_pump[cab_idx] = handler;

        connect(azv_fuel_pump[cab_idx], &ControlHandler::sigOnValueChanged,
                this, &TEP70IOController::slotOnControlValueChanged);
    }

    switch_KB1 = control_handlers[cabs_num].getByKey1(CTRL_VVK_KB1).value();
    switch_KB2 = control_handlers[cabs_num].getByKey1(CTRL_VVK_KB2).value();
    keep_fuel_pump_botton = control_handlers[cabs_num].getByKey1(CTRL_VVK_KEEP_FUEL_PUMP_BUTTON).value();

    connect(switch_KB1, &ControlHandler::sigOnValueChanged,
            this, &TEP70IOController::slotOnControlValueChanged);


}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void TEP70IOController::step(float t, float dt, const std::vector<float> *server_signals)
{
    IOController::step(t, dt, server_signals);

    for (auto cab_idx : {CAB1, CAB2})
    {
        lockReversHandle(cab_idx);
        lockMainHandle(cab_idx);
        switchCabineByKey(cab_idx);
    }
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void TEP70IOController::processKeyboardInput(std::set<uint16_t> &pressed_keys)
{

}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void TEP70IOController::lockReversHandle(int cab_idx)
{
    int main_pos = main_handle[cab_idx]->getSignalValue();

    if (main_pos != MS_ZERO)
    {
        int revers_pos = revers_handle[cab_idx]->getSignalValue();
        revers_handle[cab_idx]->setValueIfChanged(revers_pos);
    }
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void TEP70IOController::lockMainHandle(int cab_idx)
{
    int revers_pos = revers_handle[cab_idx]->getSignalValue();

    if (revers_pos == RS_ZERO)
    {
        main_handle[cab_idx]->setValueIfChanged(MS_ZERO);
    }
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void TEP70IOController::switchCabineByKey(int cab_idx)
{
    if (cabine1_switch_key[cab_idx]->toBool())
    {
        switch_KB1->setValueIfChanged(0.0f);
        switch_KB2->setValueIfChanged(0.0f);
    }

    if (cabine2_switch_key[cab_idx]->toBool())
    {
        switch_KB1->setValueIfChanged(1.0f);
        switch_KB2->setValueIfChanged(1.0f);
    }
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void TEP70IOController::slotOnControlValueChanged(const float &value)
{
    (void) value;

    // Сбрасываем удерживаемую кнопку
    keep_fuel_pump_botton->reset();
}

GET_MODULE(TEP70IOController)
