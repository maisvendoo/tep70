#ifndef     BLINDS_ACTOR_H
#define     BLINDS_ACTOR_H

#include    <device.h>

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
class BlindsActor : public Device
{
public:

    BlindsActor(QObject *parent = nullptr);

    ~BlindsActor();

    bool isOpened() const
    {
        return getY(0) >= 0.95;
    }

    void setState(bool is_open)
    {
        is_open_prev = this->is_open;
        this->is_open = is_open;
    }

    float getState() const
    {
        return static_cast<float>(getY(0));
    }

    enum
    {
        OPEN_SOUND,
        CLOSE_SOUND,
        NUM_SOUNDS
    };

    /// Состояние звука
    sound_state_t getSoundState(size_t idx = OPEN_SOUND) const override;

    float getSoundSignal(size_t idx = 0) const override;

private:

    /// Максимальный темп открытия и закрытия створок
    double max_rate = 1.0;

    /// Текущий темп открытия и закрытия створок
    double cur_rate = 0.0;

    /// Сигнал открытия
    bool is_open = false;

    bool is_open_prev = false;

    /// Состояние звука
    sound_state_t sound_state_open;
    sound_state_t sound_state_close;

    void preStep(state_vector_t &Y, double t) override;

    void ode_system(const state_vector_t &Y,
                    state_vector_t &dYdt,
                    double t) override;

    void load_config(CfgReader &cfg) override;
};

#endif // BLINDS_ACTOR_H
