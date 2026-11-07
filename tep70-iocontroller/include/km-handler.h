#ifndef     KM_HANDLER_H
#define     KM_HANDLER_H

#include    <switcher-handler.h>

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
class HandlerKM2202 : public SwitcherHandler
{
public:

    HandlerKM2202(QObject *parent = nullptr);

    virtual bool load_config(CfgReader &cfg, QDomNode secNode) override;

    virtual void processKeyInput(const std::set<uint16_t> &pressed_keys) override;

    virtual void processMouseInput(uint32_t button, bool is_pressed) override;

    virtual void step(float t, float dt) override;

    QString getState() const override;

private:


};

#endif
