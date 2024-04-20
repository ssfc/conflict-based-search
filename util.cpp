//
// Created by take_ on 2023/9/7.
//
#include "util.hpp"
#include <iostream>


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


Location move_agent(Location current_location, int action_choice)
{
    if(action_choice == 0) // wait
    {
        return current_location;
    }
    else if(action_choice == 1)
    {
        // north
        return Location{current_location.x, current_location.y + 1};
    }
    else if(action_choice == 2)
    {
        // south
        return Location{current_location.x, current_location.y - 1};
    }
    else if(action_choice == 3)
    {
        // west
        return Location{current_location.x + 1, current_location.y};
    }
    else if(action_choice == 4)
    {
        // east
        return Location{current_location.x - 1, current_location.y};
    }
    else if(action_choice == 5)
    {
        // north two
        return Location{current_location.x, current_location.y + 2};
    }
    else if(action_choice == 6)
    {
        // south two
        return Location{current_location.x, current_location.y - 2};
    }
    else if(action_choice == 7)
    {
        // east two
        return Location{current_location.x + 2, current_location.y};
    }
    else if(action_choice == 8)
    {
        // west two
        return Location{current_location.x - 2, current_location.y};
    }
    else
    {
        return Location{-1, -1};
    }
}