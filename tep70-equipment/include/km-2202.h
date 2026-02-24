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

    /// Разрешить установить реверсивку (для реализации одной рукоятки на несколько кабин)
    void allowReversHandle(bool allow);

    /// Разрешение установить реверсивку (для реализации одной рукоятки на несколько кабин)
    bool isReversHandleAllowed() const;

    /// Вставить/извлечь реверсивную рукоятку
    void insertReversHandle(bool insert);

    /// Признак вставленной реверсивной рукоятки
    bool isReversHandle() const;

    /// Вернуть текущую позицию реверсивки
    std::int8_t getReversState() const { return rs_position; }

    /// Вернуть текущую позицию штурвала
    std::int8_t getPositionNumber() const { return ms_position; }

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
        NUM_SOUNDS = 2,
        MAIN_SHAFT = 0,
        REVERS_SHAFT = 1,
        HANDLE_CHANGE_SOUND = NUM_SOUNDS + Trigger::CHANGE_SOUND,
        HANDLE_INSERTED_SOUND = NUM_SOUNDS + Trigger::ON_SOUND,
        HANDLE_REMOVED_SOUND = NUM_SOUNDS + Trigger::OFF_SOUND
    };

    float getSoundSignal(size_t idx = 0) const override;

    void setReversFwd()
    {
        rs_dir = 1;
        slotRotateReversShaft();
    }

    void setReversBwd()
    {
        rs_dir = -1;
        slotRotateReversShaft();
    }

    void setPos(int8_t pos)
    {
        ms_position = cut(pos, static_cast<int8_t>(MS_ZERO), static_cast<int8_t>(MS_MAX_POSITION));
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

    double  ms_delay = 0.2;

    double  rs_delay = 0.2;

    /// Разрешение установить реверсивку (для реализации одной рукоятки на несколько кабин)
    bool    is_reverse_handle_allowed = true;

    /// Контакт "Вперед" реверсивного вала
    bool    is_forward = false;

    /// Котакт "Назад" реверсивного вала
    bool    is_backward = false;

    /// Положение реверсивного вала
    std::int8_t rs_position = RS_ZERO;

    /// Направление вращения реверсивного вала
    std::int8_t rs_dir = 0;

    /// Положение главного вала
    std::int8_t ms_position = MS_ZERO;

    /// Направление вращения главного вала
    std::int8_t ms_dir = 0;

    /// Признак реверсивной рукоятки
    Trigger is_revers_handle;

    Timer   main_shaft_timer = Timer(ms_delay, true);

    Timer   revers_shaft_timer = Timer(rs_delay, true);

    QMap<std::int8_t, double>   n_ref;

    std::array<sound_state_t, 2> sound_states;

    void preStep(state_vector_t &Y, double t) override;

    void ode_system(const state_vector_t &Y, state_vector_t &dYdt, double t) override;

    void load_config(CfgReader &cfg) override;

    void stepKeysControl(double t, double dt) override;

private slots:

    void slotRotateMainShaft();

    void slotRotateReversShaft();
};

#endif // KM2202_H
