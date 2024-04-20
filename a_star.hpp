//
// Created by take_ on 2023/7/17.
//

#ifndef A_STAR_A_STAR_H
#define A_STAR_A_STAR_H

#include <climits>
#include <cmath>
#include <cstring>
#include <fstream>
#include <iostream>
#include <queue>
#include <set>
#include <stack>
#include <unordered_set>
#include <utility>
#include <vector>

#include <boost/heap/d_ary_heap.hpp>

#include "util.hpp"


// f_score属于AStarNode, 因为要在OpenQueue中排序，g_score属于cell, 因为要标记INF.
class AStarNode
{
public:
    Location location;
    int g_score; // cost of the cheapest path from start to n currently known
    int f_score; // g+h

public:
    AStarNode(Location input_location, int input_g_score, int input_f_score) :
        location(input_location),
        g_score(input_g_score),
        f_score(input_f_score)
    {}

    bool operator<(const AStarNode& other) const
    {
        // 反向代表最小值放堆顶。
        return std::tie(f_score, location.x, location.y)
               > std::tie(other.f_score, other.location.x, other.location.y);
    }
};

class AStar
{
private:
    // member var
    int num_columns;
    int num_rows;
    std::unordered_set<Location> obstacles;
    Location start;
    Location goal;

    // debug var;
    int iter;
    double start_time;
    size_t num_expanded_nodes;
    size_t num_generated_nodes;

    using HeapHandle = typename boost::heap::d_ary_heap<AStarNode, boost::heap::arity<2>, boost::heap::mutable_<true>>
    ::handle_type;
public:
    AStar(Location input_src, Location input_dest);

    // check whether given Cell (row, col) is a valid Cell or not.
    [[nodiscard]] bool location_valid(Location location) const;

    // check whether the given Cell is blocked or not
    bool is_passable(Location location);

    // check whether destination Cell has been reached or not
    [[nodiscard]] bool is_goal(Location location) const;

    // calculate the 'h_score' multi_agent_h_values.
    [[nodiscard]] int calculate_h(Location location) const;

    // generate children for current node
    static std::vector<Location> get_neighbors(const Location& current_location);

    // trace the path from the source to destination
    void trace_path();

    // find the shortest path between a given source Cell to a destination Cell
    bool a_star_search();

    // debug func
    void output_map();
};


#endif //A_STAR_A_STAR_H