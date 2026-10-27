#ifndef     TEP70_IO_CONTROLLER_H
#define     TEP70_IO_CONTROLLER_H

#include    <io-controller.h>

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
class TEP70IOController : public IOController
{
public:

    TEP70IOController();

    ~TEP70IOController() = default;

    void init() override;

    void step(float t, float dt, const std::vector<float> *server_signals) override;

private:

    enum
    {
        CAB1 = 0,
        CAB2 = 1,
        CABS_NUM
    };

    ControlHandler  *revers_handle[CABS_NUM] = {nullptr, nullptr};

    ControlHandler  *main_handle[CABS_NUM] = {nullptr, nullptr};

    ControlHandler *cabine1_switch_key[CABS_NUM] = {nullptr, nullptr};

    ControlHandler *cabine2_switch_key[CABS_NUM] = {nullptr, nullptr};

    ControlHandler *azv_fuel_pump[CABS_NUM] = {nullptr, nullptr};

    ControlHandler *switch_KB1 = nullptr;

    ControlHandler *switch_KB2 = nullptr;

    ControlHandler *keep_fuel_pump_botton = nullptr;

    void processKeyboardInput(std::set<uint16_t> &pressed_keys) override;

    void lockReversHandle(int cab_idx);

    void lockMainHandle(int cab_idx);

    void switchCabineByKey(int cab_idx);

private slots:

    void slotOnControlValueChanged(const float &value);
};

#endif
