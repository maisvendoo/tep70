#ifndef     KLUB_DISPLAY
#define     KLUB_DISPLAY

#include    "display.h"

#include    "led-lamp.h"

#include    <QTimer>

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
class KlubDisplay : public AbstractDisplay
{
public:

    KlubDisplay(QWidget *parent = Q_NULLPTR,
                Qt::WindowFlags f = Qt::WindowFlags());

    ~KlubDisplay() override;

    void init() override;

    void update(double t, double dt) override;

private:

    double upd_interval = 0.1;
    double upd_time = 0.0;

    bool alarm_state;

    LEDLamp *alarm;
};

#endif
