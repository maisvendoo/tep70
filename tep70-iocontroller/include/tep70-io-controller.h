#ifndef     TEP70_IO_CONTROLLER_H
#define     TEP70_IO_CONTROLLER_H

#include    <io-controller.h>
#include    <km-2202.h>

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

    ControlHandler *create_handler(QString type, QDomNode secNode, CfgReader &cfg) override;

    enum
    {
        CAB1 = 0,
        CAB2 = 1,
        CABS_NUM
    };

    ControlHandler *revers_handle_holder[CABS_NUM] = {nullptr, nullptr};

    ControlHandler  *revers_handle[CABS_NUM] = {nullptr, nullptr};

    ControlHandler  *main_handle[CABS_NUM] = {nullptr, nullptr};

    ControlHandler *cabine1_switch_key[CABS_NUM] = {nullptr, nullptr};

    ControlHandler *cabine2_switch_key[CABS_NUM] = {nullptr, nullptr};

    ControlHandler *azv_fuel_pump[CABS_NUM] = {nullptr, nullptr};

    ControlHandler *switch_KB1 = nullptr;

    ControlHandler *switch_KB2 = nullptr;

    ControlHandler *keep_fuel_pump_botton = nullptr;

    ControlHandler *edb_check_button[CABS_NUM] = {nullptr, nullptr};

    int km_ms_pos_prev = MS_POS_0;

    void processKeyboardInput(std::set<uint16_t> &pressed_keys) override;

    void lockReversHandle(int cab_idx);

    void lockMainHandle(int cab_idx);

    void switchCabineByKey(int cab_idx);

private slots:

    void slotOnFuelPumpControlValueChanged(const float &value);

    void slotOnKMControlValueChanded(const float &value);
};

#endif
