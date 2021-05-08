#include <person.hpp>

//Constructor
Person::Person(State condition, Position pos, State new_condition)
: condition{condition}, pos{pos}, new_condition{new_condition}
{}
//Default constructor
/*Person::Person()
: condition{}, pos{}, newcondition{}
{}*/

Position* Person::get_pos()
{
    return &pos;
}

State Person::get_condition()
{
    return condition;
}

void Person::next_condition()
{
    switch(new_condition){
    case State::S: new_condition = State::E;
        break;
    case State::E: new_condition = State::I;
        break;
    case State::I: new_condition = State::R;
        break;
    }
}




