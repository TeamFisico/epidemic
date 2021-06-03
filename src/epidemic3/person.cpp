#include "person.hpp"
#include "simulation.hpp"
#include <random>

namespace SMOOTH
{
Person::Person()
{
    status = Status::Susceptible;
    label = 0;
    home = {0.0, 0.0};
    location = {0.0, 0.0};
}
double Person::speed() const
{
    return sqrt(velocity[0] * velocity[0] + velocity[1] * velocity[1]);
}
bool Person::at_home()
{
    if (location == home && *target_location == home) return true;
    return false;
}
/////////////////////////////////////////////////////
////////            SPEED UPDATE              ///////
/////////////////////////////////////////////////////
void update_speed(Person* person)
{
    const double max_std_dev = 1; // TODO Determine maximum standard deviation
    // velocity is measured in units/s
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> std_dev(0, max_std_dev); // set the std dev extraction

    std::normal_distribution<double> x_sp(person->speed_x(), std_dev(gen));
    std::normal_distribution<double> y_sp(person->speed_y(), std_dev(gen));

    person->set_speed_x(x_sp(gen));
    person->set_speed_y(y_sp(gen));
}

/////////////////////////////////////////////////////
////////  MOVE A PERSON TOWARD A LOCATION     ///////
/////////////////////////////////////////////////////
void move_toward(Person* person, Location target)
// move a person by an amount determined by the current speed
{
    double x_displacement = person->speed_x() * TIME_STEP; // x = v_x * ∆t
    double y_displacement = person->speed_y() * TIME_STEP; // y = v_y * ∆t
    double displacement = sqrt(x_displacement * x_displacement + y_displacement * y_displacement);

    if (person->get_location().get_distance(target) < displacement)
    {
        person->set_location(target);
        //        person->at_place = true;
        update_speed(person);
        return;
    }

    double delta_x = std::abs(person->get_location().X() - target.X());
    double delta_y = std::abs(person->get_location().Y() - target.Y());
    double theta = atan(delta_y / delta_x); // angle connecting the target through a straight line

    std::random_device rd;
    std::mt19937 gen(rd());
    // generate an angle between velocity vector direction and the target's one so that will not point precisely to the
    // target
    std::uniform_real_distribution<double> angle(person->direction(), theta);

    double final_angle = angle(gen);
    // calculate new displacement
    // replace the precvious velocity vector with a new one with the same magnitude but different direction
    // so recalculating the new v_x and v_y to determine x_displacement and y_displacement
    x_displacement = person->speed() * cos(final_angle) * TIME_STEP;
    y_displacement = person->speed() * sin(final_angle) * TIME_STEP;

    Location new_location{person->get_location().X() + x_displacement, person->get_location().Y() + y_displacement};

    person->set_location(new_location);

    update_speed(person); // set new person speed
}

} // namespace SMOOTH