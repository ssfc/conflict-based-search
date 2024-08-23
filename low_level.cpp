//
// Created by take_ on 2023/8/22
//
#include "low_level.hpp"

#include <utility>

using namespace std;

LowLevel::LowLevel(int input_num_columns,
                   int input_num_rows,
                   std::unordered_set<Location> input_obstacles,
                   std::vector<Location> input_starts,
                   std::vector<Location> input_goals,
                   std::vector<std::unordered_map<Location, int>> input_multi_agent_h_values,
                   int input_agent_id,
                   AgentConstraints input_constraints):
        num_columns(input_num_columns),
        num_rows(input_num_rows),
        obstacles(std::move(input_obstacles)),
        starts(std::move(input_starts)),
        goals(std::move(input_goals)),
        multi_agent_h_values(std::move(input_multi_agent_h_values)),
        agent_id(input_agent_id),
        agent_constraints(std::move(input_constraints))
{
    start = starts[agent_id];
    goal = goals[agent_id];
    agent_h_values = multi_agent_h_values[agent_id];
    max_goal_constraint_time = agent_constraints.max_goal_constraint_time;
}

void LowLevel::set_low_level_context(int input_agent_id,
     AgentConstraints input_constraints)
{
    agent_id = input_agent_id;
    agent_constraints = std::move(input_constraints);

    start = starts[agent_id];
    goal = goals[agent_id];
    agent_h_values = multi_agent_h_values[agent_id];

    // reset process var.
    // agent_time_constraints.clear();
    max_goal_constraint_time = agent_constraints.max_goal_constraint_time;
}

int LowLevel::calculate_h(const Location& location) const
{
    return abs(location.x - goal.x) + abs(location.y - goal.y);
    // return agent_h_values[location];
}

bool LowLevel::is_element_of_vertex_constraints(int next_time_step, Location next_location)
{
    return agent_constraints.vertex_constraints.find(TimeLocation(next_time_step, next_location))
        != agent_constraints.vertex_constraints.end();
}

bool LowLevel::is_element_of_edge_constraints(Location current_location, Location next_location, int next_time_step)
{
    // if next_time_step not in self.agent_time_constraints:
    // find==是not element of, find!=是element of
    return agent_constraints.edge_constraints.find(EdgeConstraint(next_time_step, current_location, next_location))
           != agent_constraints.edge_constraints.end();
}

// low level 工具函数
bool LowLevel::is_solution(TimeLocation current_time_location)
{
    if(current_time_location.location == goal && current_time_location.time_step > max_goal_constraint_time)
    {
        return true;
    }

    return false;
}

bool LowLevel::is_in_range(const Location& location) const
{
    if(location.x >= 0 && location.x < num_columns
    && location.y >= 0 && location.y < num_rows)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool LowLevel::is_obstacle(const Location& location) const
{
    if(obstacles.find(location) != obstacles.end())
    {
        return true;
    }
    else
    {
        return false;
    }
}

std::vector<TimeLocation> LowLevel::get_neighbors(TimeLocation current_time_location)
{
    vector<TimeLocation> neighbor_time_locations;
    // directions for move() for each agent: 0, 1, 2, 3, 4
    // 虽然在util.cpp中也提供了south two的选项, 但是这里只选择了前5个, 所以就只有前进一步的了
    for(int i=0;i<5;i++)
    {
        auto neighbor_location = move_agent(current_time_location.location, i);
        // cerr << "neighbor_location: (" << neighbor_location.y << "," << neighbor_location.x << ")" << endl;
        // check whether neighbor locations is out of map range.
        // check whether neighbor locations is passable.

        if(is_in_range(neighbor_location)
        && !is_obstacle(neighbor_location)
        && !is_element_of_vertex_constraints(current_time_location.time_step + 1,
                                             neighbor_location)
        && !is_element_of_edge_constraints(current_time_location.location, neighbor_location,
                                             current_time_location.time_step + 1))
        {
            neighbor_time_locations.emplace_back(current_time_location.time_step + 1, neighbor_location);
        }
    }

    return neighbor_time_locations;
}

// low_level_search和a_star算法的重要差别是，low_level_search需要考虑时间维度。
// A* LINE 1
// A* finds a path from start to goal.
// h_score is the heuristic function. h_score(n) estimates the cost to reach goal from node n.
// function A_Star(start, goal, h_score)
// find the shortest path between a given source cell to a destination cell
bool LowLevel::low_level_search(vector<TimeLocation>& solution_path, size_t& num_expanded_low_level_nodes)
{
    // cerr << "max_constraint_time_step: " << max_constraint_time_step << endl;

    // A* LINE 2
    // For node n, came_from_list[n] is the node immediately preceding it on the cheapest path from the start
    // to n currently known.
    // came_from_list := an empty map
    std::unordered_map<TimeLocation, TimeLocation, std::hash<TimeLocation>> came_from;

    // A* LINE 3
    // For node n, g_score[n] is the cost of the cheapest path from start to n currently known.
    // g_score := map with default value of Infinity
    using OpenHeap = boost::heap::d_ary_heap<LowLevelNode, boost::heap::arity<2>, boost::heap::mutable_<true>>;
    using HeapHandle = typename OpenHeap::handle_type;
    OpenHeap open_set;
    std::unordered_map<TimeLocation, HeapHandle, std::hash<TimeLocation>> timelocation_to_heaphandle;

    // A* LINE 4
    // g_score[start] := 0

    // A* LINE 5
    // For node n, f_score[n] := g_score[n] + h(n). f_score[n] represents our current best guess as to
    // how cheap a path could be from start to finish if it goes through n.
    // f_score := map with default value of Infinity

    // A* LINE 6
    // f_score[start] := h(start)

    auto root = LowLevelNode(TimeLocation(0, start),
                             calculate_h(start), 0);
    // cerr << "root: "; print_low_level_list_node(root);

    // A* LINE 7
    // The set of discovered nodes that may need to be (re-)expanded.
    // Initially, only the start node is known.
    // This is usually implemented as a min-heap or priority queue rather than a hash-set.
    // open_set := {start}
    auto root_handle = open_set.emplace(root);
    timelocation_to_heaphandle.insert(std::make_pair<>(TimeLocation(0, start), root_handle));

    // A* LINE 8
    // node that has already been evaluated. In other words, already been poped from open_set.
    // closed_set := the empty set
    std::unordered_set<TimeLocation, std::hash<TimeLocation>> closed_set;

    // A* LINE 9
    // while open_set is not empty
    // int iter_low_level = 0;
    while (!open_set.empty())
    // while(!open_set.empty() && iter_low_level < 5)
    {
        // A* LINE 10
        // This operation can occur in O(Log(N)) time if open_set is a min-heap or a priority queue
        // current := the node in open_set having the lowest f_score[] value
        auto current = open_set.top();
        // cerr << "iter_low_level: " << iter_low_level << endl;
        // print_low_level_list(open_set);
        // cerr << "current node: "; print_low_level_list_node(current);
        num_expanded_low_level_nodes++;

        // A* LINE 11
        // if current = goal
        if (is_solution(current.time_location))
        {
            solution_path.clear();
            // A* LINE 12
            // total_path := {current}
            solution_path.emplace(solution_path.begin(), came_from.size(), goal);

            // A* LINE 13
            // while current in cameFrom.Keys:
            auto pair_found = came_from.find(current.time_location);
            while (pair_found != came_from.end())
            {
                // A* LINE 14
                // current := came_from[current]

                // A* LINE 15
                // total_path.prepend(current)

                // iter->first是TimeLocation
                // iter->second是TimeLocation
                solution_path.emplace(solution_path.begin(), pair_found->second);
                pair_found = came_from.find(pair_found->second); // find(Location)
            }

            // A* LINE 16
            // return total_path
            return true;
        }

        // A* LINE 17
        // open_set.Remove(current)
        open_set.pop();

        // A* LINE 18
        // add current to closed_set.
        closed_set.insert(current.time_location);

        // A* LINE 19
        // for each neighbor of current
        auto neighbors = get_neighbors(current.time_location);
        // cerr << "neighbors:";
        for (auto neighbor : neighbors)
        {
            // print_low_level_list_node(neighbor);

            // A* LINE 20
            // if neighbor not in closed_set
            // 不在closed_set中
            if (closed_set.find(neighbor) == closed_set.end())
            {
                // A* LINE 21
                // d(current,neighbor) is the weight of the edge from current to neighbor
                // tentative_g_score is the distance from start to the neighbor through current
                // tentative_g_score := g_score[current] + d(current, neighbor)
                int tentative_g_score = current.g_score + 1;

                // A* LINE 22
                // if neighbor not in open_set
                auto pair_found = timelocation_to_heaphandle.find(neighbor);
                if (pair_found == timelocation_to_heaphandle.end())
                {
                    // A* LINE 23
                    // This path to neighbor is better than any previous one. Record it!
                    // came_from[neighbor] := current
                    came_from.insert(std::make_pair<>(neighbor, current.time_location));

                    // A* LINE 24
                    // g_score[neighbor] := tentative_g_score
                    // cerr << "g_score in generate: " << g_score << endl;

                    // A* LINE 25
                    // f_score[neighbor] := tentative_g_score + h(neighbor)
                    int f_new = tentative_g_score + calculate_h(neighbor.location);

                    // A* LINE 26
                    // open_set.add(neighbor)
                    auto neighbor_handle
                        = open_set.emplace(LowLevelNode(neighbor, tentative_g_score, f_new));
                    timelocation_to_heaphandle.insert(std::make_pair<>(neighbor, neighbor_handle));
                }
                // A* LINE 27
                // else // neighbor in open_set
                else
                {
                    auto neighbor_handle = pair_found->second;
                    // A* LINE 28
                    // This path to neighbor is better than any previous one. Record it!
                    // if tentative_g_score < g_score[neighbor]
                    if (tentative_g_score < (*neighbor_handle).g_score) // new path is better
                    {
                        // A* LINE 29
                        // came_from[neighbor] := current
                        came_from[neighbor] = current.time_location;
                        // A* LINE 30
                        // g_score[neighbor] := tentative_g_score
                        // A* LINE 31
                        // f_score[neighbor] := tentative_g_score + h(neighbor)
                        // A* LINE 32
                        // open_set.update(neighbor)
                        (*neighbor_handle).g_score = tentative_g_score;
                        (*neighbor_handle).f_score = tentative_g_score + calculate_h(neighbor.location);

                        open_set.increase(neighbor_handle);
                    }
                }

            }

        }

        // iter_low_level++;
    }

    // cerr << "no solution" << endl;

    // A* LINE 33
    // Open set is empty but goal was never reached
    // return failure
    return false;
}




