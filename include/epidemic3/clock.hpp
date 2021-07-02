#ifndef CLOCK_HPP
#define CLOCK_HPP

#include <chrono>

namespace smooth_simulation
{
class Clock
{
  private:
    int day;
    int hours;
    int minutes;

  public:
    Clock();
    void update(int time_step_in_minutes);
};

} // namespace smooth_simulation





#endif // CLOCK_HPP