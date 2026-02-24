#ifndef     TEP70_AUTOPILOT_TYPES_H
#define     TEP70_AUTOPILOT_TYPES_H

#include    <autopilot-types.h>
#include    <cstdint>

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
class tep70_control_t : public auto_control_t
{
public:

    int8_t km_pos = 0;

    int krm_pos = 1;

    double kvt_pos = 0.0;

    tep70_control_t() : auto_control_t()
    {

    }
};

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
class tep70_feedback_t : public auto_feedback_t
{
public:

    double I_gen = 0.0;

    bool is_EPB_on = false;

    tep70_feedback_t() : auto_feedback_t()
    {

    }
};

#endif
