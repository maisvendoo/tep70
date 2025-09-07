#include    "tep70bs.h"


void TEP70BS::initAutostart()
{
    start_count = 0;

    triggers.push_back(&azv_common_control[CAB1]);
    triggers.push_back(&azv_fuel_pump[CAB1]);
    triggers.push_back(&button_disel_start[CAB1]);
    triggers.push_back(&azv_upr_tepl[CAB1]);
    triggers.push_back(&azv_ept_on[CAB1]);

    connect(&autoStartTimer, &Timer::process, this, &TEP70BS::slotAutostart);
    autoStartTimer.firstProcess(true);
    autoStartTimer.setTimeout(0.5);
}

void TEP70BS::stepAutostart(const double& t, const double& dt)
{
    autoStartTimer.step(t, dt);
}


void TEP70BS::slotAutostart()
{
    if (start_count < triggers.size())
    {
        triggers[start_count]->set();

        if (!kontaktor_oil_pump->getContactState(1) &&
             (triggers[start_count] == &button_disel_start[CAB1]) )
        {
            return;
        }
        else
        {
            button_disel_start[CAB1].reset();

            if (!ru10->getContactState(1) && kontaktor_oil_pump->getContactState(1))
                return;
        }

        start_count++;
    }
    else
    {
        autoStartTimer.stop();
        tumbler_revers[CAB1].setPosition(2);
    }
}
