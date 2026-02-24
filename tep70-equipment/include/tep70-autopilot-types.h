#ifndef     TEP70_AUTOPILOT_TYPES_H
#define     TEP70_AUTOPILOT_TYPES_H

#include    <autopilot-types.h>

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
class tep70_control_t : public auto_control_t
{
public:

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

    tep70_feedback_t() : auto_feedback_t()
    {

    }
};

#endif
