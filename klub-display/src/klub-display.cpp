#include    "klub-display.h"

#include    <QLabel>
#include    <QLayout>

#include    "tep70bs-signals.h"

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
KlubDisplay::KlubDisplay(QWidget *parent, Qt::WindowFlags f) : AbstractDisplay(parent, f)
  , alarm_state(false)
{
    this->setWindowFlag(Qt::WindowType::FramelessWindowHint);
    this->resize(2048, 638);
    this->setAutoFillBackground(true);
    this->setPalette(QPalette(QColor(255, 255, 255)));
    this->setAttribute(Qt::WA_TransparentForMouseEvents);

    this->setLayout(new QVBoxLayout);
    this->setFocusPolicy(Qt::FocusPolicy::NoFocus);
    this->layout()->setContentsMargins(0, 0, 0, 0);
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
KlubDisplay::~KlubDisplay()
{

}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void KlubDisplay::init()
{
    QLabel *background = new QLabel(this);
    background->setFrameShape(QLabel::NoFrame);

    QPixmap pic;

    if (!pic.load(":/klub/img-background"))
    {
        return;
    }

    background->setFixedWidth(pic.width());
    background->setFixedHeight(pic.height());
    background->setPixmap(pic);

    // Лампа проверки бдительности (треугольная)
    alarm = new LEDLamp(background);
    alarm->setOnImage(":/klub/alarm_on");
    alarm->setOffImage(":/klub/alarm_off");
    alarm->setPosition(1935, 137);

    this->layout()->addWidget(background);

    AbstractDisplay::init();
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void KlubDisplay::update(double t, double dt)
{
    (void) t;

    // Интервал обновления
    upd_time += dt;
    if ((upd_time < upd_interval) || (signal_id < 0))
    {
        need_repaint = false;
        return;
    }

    input_signals.resize(SIGNALS_NUM_TOTAL, 0.0f);
    need_repaint = true;
    upd_time = 0.0;

    if (!TO_BOOL(input_signals[KLUB_ON]))
        return;

    alarm->setState(TO_BOOL(input_signals[KLUB_ALARM]));
}

GET_DISPLAY(KlubDisplay)
