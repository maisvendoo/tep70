#include <km-handler.h>

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
HandlerKM2202::HandlerKM2202(QObject *parent) : SwitcherHandler(parent)
{

}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
bool HandlerKM2202::load_config(CfgReader &cfg, QDomNode secNode)
{
    SwitcherHandler::load_config(cfg, secNode);

    return true;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void HandlerKM2202::processKeyInput(const std::set<uint16_t> &pressed_keys)
{
    SwitcherHandler::processKeyInput(pressed_keys);
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void HandlerKM2202::processMouseInput(uint32_t button, bool is_pressed)
{
    SwitcherHandler::processMouseInput(button, is_pressed);
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void HandlerKM2202::step(float t, float dt)
{
    SwitcherHandler::step(t, dt);
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
QString HandlerKM2202::getState() const
{
    return SwitcherHandler::getState();
}
