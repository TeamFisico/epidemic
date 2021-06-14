#include "clock.hpp"

namespace smooth_simulation
{
Clock::Clock()
{
}

void Clock::update(int time_step_in_minutes)
{
    if (minutes + time_step_in_minutes >= 60) // need to change hour
    {
        if (hours == 24) // need to change day
        {
            ++day;
            hours = 0;
            minutes = minutes + time_step_in_minutes - 60;
        }
        else
        {
            ++hours;
            minutes = minutes + time_step_in_minutes - 60;
        }
    }
    else // no hour change
    {
        minutes += time_step_in_minutes;
    }
}

} // namespace smooth_simulation
