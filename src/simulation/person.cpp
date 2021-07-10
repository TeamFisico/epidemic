#include <person.hpp>

namespace smooth_sim
{

////////////////////////////////////////////////////////
/////              PERSON CONSTRUCTOR             //////
////////////////////////////////////////////////////////
Person::Person(Status current_status, Position position, Status new_status, Location home, int cluster_index)
    : status{current_status},
      new_status{new_status},
      pos{position},
      home{home},
      cluster_index{cluster_index}
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////           PUBLIC METHODS            /////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////// REFERENCE TO AREA OF THE CLUSTER /////////////////
Status Person::get_current_status() const
{
    return status;
}
///////////////// PERSON POSITION /////////////////
Position Person::get_position() const
{
    return pos;
}
///////////////// IS THE PERSON AT HIS/HER TARGET LOCATION? /////////////////
bool Person::at_location(Location* loc) const
{
    return (pos.in_radius(loc->get_position(), loc->get_radius()));
}
///////////////// PERSON LABEL(BELONGING CLUSTER LABEL(INDEX)) /////////////////
int Person::get_label() const
{
    return cluster_index;
}
///////////////// REFERENCE TO PERSON POSITION /////////////////
Position& Person::position()
{
    return pos;
}
///////////////// POINTER TO PERSON HOME LOCATION /////////////////
Location* Person::get_home()
{
    return &home;
}
///////////////// SET CURRENT PERSON STATUS /////////////////
void Person::set_current_status(Status current_person_status)
{
    status = current_person_status;
}
///////////////// SET NEW PERSON STATUS /////////////////
void Person::set_new_status(Status new_person_status)
{
    new_status = new_person_status;
}
///////////////// UPDATE PERSON STATUS  /////////////////
void Person::update_status()
{
    status = new_status;
}

} // namespace smooth_sim