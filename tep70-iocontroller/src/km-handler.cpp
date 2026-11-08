#include    <km-handler.h>
#include    <km-2202.h>

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

    double tmp = 0;
    cfg.getDouble(secNode, "SelsinSpeed", tmp);
    selsin_speed = static_cast<float>(tmp);

    return true;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void HandlerKM2202::processKeyInput(const std::set<uint16_t> &pressed_keys)
{
    SwitcherHandler::processKeyInput(pressed_keys);

    if (getKeyState(pressed_keys, keyCodeInc) && isKeyModifier(pressed_keys, keyModIncName))
    {
        selsin_dir = +1;
        return;
    }

    if (getKeyState(pressed_keys, keyCodeDec) && isKeyModifier(pressed_keys, keyModDecName))
    {
        selsin_dir = -1;
        return;
    }

    selsin_dir = 0;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void HandlerKM2202::processMouseInput(uint32_t button, bool is_pressed)
{
    SwitcherHandler::processMouseInput(button, is_pressed);

    if (!is_pressed)
    {
        selsin_dir = 0;
        return;
    }

    if (button == getButtonCode(incButtonName))
    {
        selsin_dir = +1;
        return;
    }

    if (button == getButtonCode(decButtonName))
    {
        selsin_dir = -1;
        return;
    }
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void HandlerKM2202::step(float t, float dt)
{
    SwitcherHandler::step(t, dt);

    // Если стоим на первой тормозной позиции
    if (toInt() < MS_EDB_150kmh)
    {
        if (params[PARAM_SELSIN_POS] > -1.0f)
        {
            value = MS_EDB_150kmh;
            params[PARAM_SELSIN_POS] += selsin_speed * selsin_dir * dt;
            params[PARAM_SELSIN_POS] = std::clamp(params[PARAM_SELSIN_POS], -1.0f, 0.0f);
            sendControlSignal();
        }
    }

    if (toInt() > MS_EDB_150kmh)
    {
        if (params[PARAM_SELSIN_POS] < 0.0f)
        {
            value = MS_EDB_150kmh;
            params[PARAM_SELSIN_POS] += selsin_speed * selsin_dir * dt;
            params[PARAM_SELSIN_POS] = std::clamp(params[PARAM_SELSIN_POS], -1.0f, 0.0f);
            sendControlSignal();
        }
    }
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
QString HandlerKM2202::getState() const
{
    return SwitcherHandler::getState() + QString(" | Сельсин: %1").arg(params[PARAM_SELSIN_POS], 5, 'f', 3);
}
