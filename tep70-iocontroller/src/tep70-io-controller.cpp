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
    }
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

GET_MODULE(TEP70IOController)