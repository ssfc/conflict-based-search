// A c++ implementation for conflict-based search.

#include <boost/program_options.hpp>

#include "a_star.hpp"


using namespace std;


// Driver program to test above function
int main(int argc, char *argv[])
{
    /* Description of the Grid-
    1--> The Cell is not blocked
    0--> The Cell is blocked */
    /*
    std::vector<std::vector<int>> test_map
            = { { 1, 0, 1, 1, 1, 1, 0, 1, 1, 1 },
                { 1, 1, 1, 0, 1, 1, 1, 0, 1, 1 },
                { 1, 1, 1, 0, 1, 1, 0, 1, 0, 1 },
                { 0, 0, 1, 0, 1, 0, 0, 0, 0, 1 },
                { 1, 1, 1, 0, 1, 1, 1, 0, 1, 0 },
                { 1, 0, 1, 1, 1, 1, 0, 1, 0, 0 },
                { 1, 0, 0, 0, 0, 1, 0, 0, 0, 1 },
                { 1, 0, 1, 1, 1, 1, 0, 1, 1, 1 },
                { 1, 1, 1, 0, 0, 0, 1, 0, 0, 1 } };
                */

    ///*  A* test
    // Source is the left-most bottom-most corner
    auto test_src = Location{0, 8};

    // Destination is the left-most top-most corner
    auto test_dest = Location{4, 2};
    // auto test_dest = Location{2, 5};

    AStar test_astar(test_src, test_dest);
    // test_astar.output_map();
    test_astar.a_star_search();
     //*/

    return 0;
}

// Test on ubuntu platform:
// cmake . ; make
// ./a_star