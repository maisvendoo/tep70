//------------------------------------------------------------------------------
//
//      Контроллер машиниста КМ 2202
//
//
//------------------------------------------------------------------------------
#ifndef     KM_2202_H
#define     KM_2202_H

#include    "device.h"
#include    "timer.h"

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
class ControllerKM2202 : public Device
{
public:

    ControllerKM2202(QObject *parent = Q_NULLPTR);

    ~ControllerKM2202();

    /// Вернуть текущую позицию штурвала
    int getPositionNumber() const { return ms_position; }

    /// Вернуть текущую позицию реверсивки
    int getReversState() const { return rs_position; }

    /// Вернуть положение главного вала
    float getMainShaftPos() const;

    /// Контакт нулевой позиции
    bool isZero() const { return ms_position == MS_ZERO; }

    /// Контакт всех ненулявых позиций
    bool isNoZero() const { return ms_position != MS_ZERO; }

    /// Контакт позиций выше 1-й
    bool isMoreFirst() const {return ms_position > 1; }

    /// Контакт позиций выше 12
    bool is12orMore() const {return ms_position >= 12; }

    /// Получить заданную частоту вращения коленчатого вала дизеля
    double getRefFreq() const { return n_ref[ms_position]; }

    /// Получить состояние контакта "Вперед"
    bool isForward() const { return rs_position == RS_FORWARD; }

    /// Получить состояние контакта "Назад"
    bool isBackward() const { return rs_position == RS_BACKWARD; }

    enum
    {
        MAIN_SHAFT = 0,
        REVERS_SHAFT = 1
    };

    float getSoundSignal(size_t idx = 0) const override
    {
        return sound_states[idx].createSoundSignal();
    }

private:

    enum
    {
        MS_ZERO = 0,
        MS_MAX_POSITION = 15
    };

    enum
    {
        RS_FORWARD = 1,
        RS_ZERO = 0,
        RS_BACKWARD = -1
    };

    double  ms_delay;

    double  rs_delay;

    /// Положение главного вала
    int     ms_position;

    /// Направление вращения главного вала
    int     ms_dir;

    /// Положение реверсивного вала
    int     rs_position;

    /// Направление вращения реверсивного вала
    int     rs_dir;    

    /// Контакт "Вперед" реверсивного вала
    bool    is_forward;

    /// Котакт "Назад" реверсивного вала
    bool    is_backward;

    Timer   main_shaft_timer;

    Timer   revers_shaft_timer;

    QMap<int, double>   n_ref;

    std::array<sound_state_t, 2> sound_states;

    void preStep(state_vector_t &Y, double t);

    void ode_system(const state_vector_t &Y, state_vector_t &dYdt, double t);

    void load_config(CfgReader &cfg);

    void stepKeysControl(double t, double dt);

private slots:

    void slotRotateMainShaft();

    void slotRotateReversShaft();
};

#endif // KM2202_H
