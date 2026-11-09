#ifndef     EDB_CONTROL_UNIT
#define     EDB_CONTROL_UNIT

#include    <device.h>

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
class ControlUnitEDB : public Device
{
public:

    ControlUnitEDB(QObject *parent = nullptr);

    ~ControlUnitEDB();

    void setPowerVoltage(double Ucc)
    {
        this->Ucc = Ucc;
    }

    bool isPowered() const
    {
        return is_power;
    }

    double getRefCurrent() const
    {
        return u_ref;//getY(1);
    }

    void setCurrentVelocity(double v)
    {
        Vkmh = qAbs(v) * Physics::kmh;
    }    

    void setAncorCurrent(double Ia)
    {
        this->Ia = Ia;
    }

    void setFieldCurrent(double If)
    {
        this->If = If;
    }

    void step(double t, double dt) override;

    void setVelocitySelsinPos(float selsin_pos)
    {
        velosity_selsin_pos = static_cast<double>(selsin_pos);
    }

    float getRefVelocityKmh() const
    {
        return static_cast<float>(V_ref);
    }

    void setForceLimitLevel(double force_limit_level)
    {
        this->force_limit_level = force_limit_level;
    }

    void fullBrake(bool is_full_brake)
    {
        this->is_full_brake = is_full_brake;
    }

    bool isReady() const
    {
        return is_brake_ready;
    }

    void stoppingBrake(bool is_stopping_brake)
    {
        this->is_stopping_brake = is_stopping_brake;
    }

    void brakePositionKM(bool is_KM_brake)
    {
        this->is_KM_brake = is_KM_brake;
    }

private:

    /// Напряжение питания блока
    double Ucc = 0.0;

    /// Признак подачи питания
    bool is_power = false;

    /// Заданный ток возбуждения ТЭД
    double u_ref = 0.0;

    /// Максимальная (конструкционная) скорость
    double Vmax = 160.0;

    /// Пердельная уставка по току якоря
    double Ia_max = 825.0;

    /// Предельная уставка по току возбуждения
    double If_max = 800.0;

    /// Текущая скорость (км/ч)
    double Vkmh = 0.0;

    /// Заданная скорость (км/ч)
    double V_ref = 0.0;

    /// Текущий ток якоря
    double Ia = 0.0;

    /// Текущий ток возбуждения
    double If = 0.0;

    /// Ошибка по ттоку якоря
    double eps_I = 0.0;

    /// Значение ошибки по току якоря на предыдущем шаге
    double eps_I_prev = 0.0;

    /// Производная ошибки по току якоря
    double deps_Idt = 0.0;

    /// Передел ограничения сигнала интегратора ошибки
    double integrator_lim = 1.0;

    /// Сигнал от сельсина-задатчика скорости
    double velosity_selsin_pos = 0.0;

    /// Уровень ограничения тормозного усилия
    double force_limit_level = 0.0;

    /// Минимальный уровень задаваемого тормозного усилия
    double min_force_level = 0.22;

    /// Шаг регулировки уровня тормозного усилия
    double force_level_step = 0.071;

    /// Уровень тормозного усилия при предварительном торможении
    double force_prep_level = 0.2;

    /// Признак отключения предварительного торможения
    bool is_full_brake = false;

    /// Минимальный рабочий ток якоря при ЭДТ
    double min_brake_current_level = 0.3;

    /// Готовность электрического тормоза
    bool is_brake_ready = false;

    /// Признак остановочного торможения
    bool is_stopping_brake = false;

    /// Скорость начала ограничения тока якоря по условиям коммутации
    double Vc = 115.0;

    /// Признак тормозной позиции КМ
    bool is_KM_brake = false;

    /// Максимальная заданная скорость торможения
    double V_ref_max = 150.0;

    enum
    {
        NUM_COEFS = 10
    };

    std::array<double, NUM_COEFS>   K;

    void preStep(state_vector_t &Y, double t) override;

    void ode_system(const state_vector_t &Y,
                    state_vector_t &dYdt,
                    double t) override;

    void load_config(CfgReader &cfg) override;
};

#endif
