#include <person.hpp>

//Constructor
Person::Person(State condition, Position pos, State newcondition)
: condition{condition}, pos{pos}, newcondition{newcondition}
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
    switch(condition){
    case State::S: condition = State::E;
        break;
    case State::E: condition = State::I;
        break;
    case State::I: condition = State::R;
        break;
    }
}




