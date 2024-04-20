//
// Created by take_ on 2023/9/7.
//
#include <iostream>
#include "util.hpp"

using namespace std;

void print_single_constraint(const NegativeConstraint& input_constraint)
{
    cerr << "NegativeConstraint: { agent: " << input_constraint.agent_id << " locations: [";

    for(auto this_location : input_constraint.locations)
    {
        cerr << "(" << this_location.x << "," << this_location.y << "),";
    }

    cerr << "], time_step: " << input_constraint.time_step << "}" << endl;
}

Location move_agent(Location current_location, int direction)
{
    if(direction == 0)
    {
        return current_location;
    }
    else if(direction == 1)
    {
        // north
        return Location{current_location.x, current_location.y + 1};
    }
    else if(direction == 2)
    {
        // south
        return Location{current_location.x, current_location.y - 1};
    }
    else if(direction == 3)
    {
        // west
        return Location{current_location.x + 1, current_location.y};
    }
    else if(direction == 4)
    {
        // east
        return Location{current_location.x - 1, current_location.y};
    }
    else
    {
        return Location{-1, -1};
    }
}