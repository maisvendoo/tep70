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

    void processKeyboardInput(std::set<uint16_t> &pressed_keys) override;

    void lockReversHandle(int cab_idx);

    void lockMainHandle(int cab_idx);
};

#endif