//
// Created by YG on 2023/7/17.
//
#include "a_star.hpp"

using namespace std;

AStar::AStar(Location input_src, Location input_dest):
        num_columns(0),
        num_rows(0),
        start(input_src),
        goal(input_dest),
        iter(0),
        start_time(clock()),
        num_expanded_nodes(0),
        num_generated_nodes(0)
// 0表示无障碍, 1表示有障碍。
{
    ifstream map_file("../instances/a_star_map.txt");
    if (map_file.is_open())
    {
        map_file >> num_rows >> num_columns;

        for (int i = 0; i < num_rows; i++)
        {
            for (int j = 0; j < num_columns; j++)
            {
                char c;
                map_file >> c;
                if(c=='@')
                {
                    obstacles.insert(Location(j, i));
                }
            }
        }

        map_file.close();

    }
    else
    {
        cerr << "Error opening file." << endl;
    }

    // A* LINE 3
    // For node n, cameFrom[n] is the node immediately preceding it on the cheapest path from the start
    // to n currently known.
    // cameFrom := an empty map

    // A* LINE 4
    // For node n, gScore[n] is the cost of the cheapest path from start to n currently known.
    // gScore := map with default value of Infinity

    // A* LINE 6
    // For node n, fScore[n] := gScore[n] + h_score(n). fScore[n] represents our current best guess as to
    // how cheap a path could be from start to finish if it goes through n.
    // fScore := map with default value of Infinity
}

// check whether given cell (row, col) is a valid cell or not.
// 也就是检查一个cell是否在地图范围内
bool AStar::location_valid(Location location) const
{
    // Returns true if row number and column number is in range
    return (location.x >= 0) && (location.x < num_columns)
           && (location.y >= 0) && (location.y < num_rows);
}

// check whether the given cell is blocked or not
bool AStar::is_passable(Location location)
{
    // Returns true if the cell is not blocked else false
    // 记得在二维数组里先y再x.
    if (obstacles.find(location) == obstacles.end())
        return true;
    else
        return false;
}

// check whether destination cell has been reached or not
bool AStar::is_goal(Location location) const
{
    if (location.x == goal.x && location.y == goal.y)
        return true;
    else
        return false;
}

// calculate the 'h_score' multi_agent_h_values.
int AStar::calculate_h(Location location) const
{
    // Return using the distance formula
    // 看样子用的是欧几里得距离。
    /*
    return ((double)sqrt(
            (location.y - goal.y) * (location.y - goal.y)
            + (location.x - goal.x) * (location.x - goal.x)));
*/
    // try manhattan distance
    return (abs(location.x - goal.x) + abs(location.y - goal.y));
}


// generate children for current node
/*
       Generating all the 4 successor of this cell

                  N
                  |
           W----cell----E
                 |
                 S

       cell-->Popped cell (current_x, current_y)
       N --> North	 (current_x, current_y+1)
       S --> South	 (current_x, current_y-1)
       W --> West	 (current_x-1, current_y)
       E --> East	 (current_x+1, current_y)
        */
std::vector<Location> AStar::get_neighbors(const Location& current_location)
{
    vector<Location> children;

    for(int i=1;i<5;i++)
    {
        // A* LINE 13
        // for each child of current
        auto child_node = move_agent(current_location, i);
        children.emplace_back(child_node);
    }

    return children;
}

// A* LINE 1
// A* finds a path from start to goal.
// h_score is the heuristic function. h_score(n) estimates the cost to reach goal from node n.
// function A_Star(start, goal, h_score)
// find the shortest path between a given source cell to a destination cell
bool AStar::a_star_search()
{
    // A* LINE 2
    // For node n, came_from_list[n] is the node immediately preceding it on the cheapest path from the start
    // to n currently known.
    // came_from_list := an empty map
    std::unordered_map<Location, Location,std::hash<Location>> came_from;

    // A* LINE 3
    // For node n, g_score[n] is the cost of the cheapest path from start to n currently known.
    // g_score := map with default value of Infinity
    boost::heap::d_ary_heap<AStarNode, boost::heap::arity<2>, boost::heap::mutable_<true>> open_set;
    std::unordered_map<Location, HeapHandle, std::hash<Location>> location_to_heaphandle;

    // A* LINE 4
    // g_score[start] := 0

    // A* LINE 5
    // For node n, f_score[n] := g_score[n] + h(n). f_score[n] represents our current best guess as to
    // how cheap a path could be from start to finish if it goes through n.
    // f_score := map with default value of Infinity

    // A* LINE 6
    // f_score[start] := h(start)
    int f_start = calculate_h(start);

    // A* LINE 7
    // openSet := {start}
    // Put the starting cell on the open list and set its 'f_score' as 0
    auto root_handle = open_set.emplace(AStarNode(start, 0, f_start));
    location_to_heaphandle.insert(std::make_pair<>(start, root_handle));

    cerr << "Add start to open set" << endl;

    // A* LINE 8
    // node that has already been evaluated. In other words, already been poped from open_set.
    // closed_set := the empty set
    std::unordered_set<Location, std::hash<Location>> closed_set;

    // A* LINE 9
    // while openSet is not empty
    while (!open_set.empty() && iter < INT_MAX)
    {
        cerr << "iter: " << iter << endl;
        // A* LINE 10
        // This operation can occur in O(Log(N)) time if open_set is a min-heap or a priority queue
        // current := the node in open_set having the lowest f_score[] value
        auto current = open_set.top();
        num_expanded_nodes++;

        // A* LINE 11
        // if current = goal
        if (is_goal(current.location))
        {
            cerr << "The destination cell is found\n";
            // A* LINE 12
            // return reconstruct_path(cameFrom, current)
            double elapsed_time = (clock() - start_time) / CLOCKS_PER_SEC;
            cerr << "success, iterations: " << iter << " elapsed_time(s): " << elapsed_time
                 << " frequency:" << double (iter) / elapsed_time << endl;

            // construct path LINE 2
            // total_path := {current}
            // current: 当前节点
            // total_path: 已经构造的路径
            // 接着创建了一个 vector 数据结构 path 用于存储找到的最优路径.
            vector<Location> path;
            path.emplace(path.begin(), goal);

            // construct path LINE 3
            // while current in cameFrom.Keys:
            auto pair_found = came_from.find(current.location);
            while (pair_found != came_from.end())
            {
                // A* LINE 14
                // current := came_from[current]

                // A* LINE 15
                // total_path.prepend(current)

                // iter->first是location
                // iter->second是std::tuple<Location,Action,int,int>
                path.emplace(path.begin(), pair_found->second);
                pair_found = came_from.find(pair_found->second); // find(Location)
            }

            // A* LINE 16
            // return total_path
            // 最后，通过另一个 while 循环遍历 path 并依次输出路径上的每个坐标 (row, col)，形式为 -> (row,col)。
            ofstream path_file("path.txt");
            if (path_file.is_open())
            {
                // construct path LINE 6
                cerr << "total cost: " << path.size() - 1 << endl;
                path_file << path.size() << "\n";
                cerr << "The path is ";
                for(const auto & element : path)
                {
                    cout << "-> (" << element.x << "," << element.y << ") ";
                    path_file << element.x << " " << element.y << "\n";
                }
            }
            else
            {
                cerr << "Unable to create path file.\n";
            }

            cerr << "num_expanded_nodes: " << num_expanded_nodes << endl;
            cerr << "num_generated_nodes: " << num_generated_nodes << endl;

            return true;
        }

        // A* LINE 17
        // openSet.Remove(current)
        // meaning: Remove this vertex from the open set
        open_set.pop();

        // A* LINE 18
        // Add this vertex to the closed list
        closed_set.insert(current.location);

        cerr << "Add smallest f(" << current.location.y << "," << current.location.x << ") = "
             << current.f_score << " to closed list" << endl;

        // A* LINE 19
        // for each neighbor of current
        auto neighbors = get_neighbors(current.location);
        for(auto neighbor : neighbors)
        {
            // Only process this cell if this is a valid one
            if (location_valid(neighbor))
            {
                // A* LINE 20
                // If the successor has not been evaluated and is passable
                if (closed_set.find(neighbor) == closed_set.end() && is_passable(neighbor))
                {
                    // A* LINE 21
                    // d(current,neighbor) is the weight of the edge from current to neighbor
                    // tentative_gScore is the distance from start to the neighbor through current
                    // tentative_gScore := gScore[current] + d(current, neighbor)
                    // cost of the cheapest path from start to n currently known
                    int tentative_g_score = current.g_score + 1;

                    // cerr << "neighbor tentative_g_score: " << tentative_g_score << " ";

                    // A* LINE 22
                    // 不在open_set中
                    auto pair_found = location_to_heaphandle.find(neighbor);
                    if (pair_found == location_to_heaphandle.end())
                    {
                        // A* LINE 23
                        // This path to neighbor is better than any previous one. Record it!
                        // came_from_list[neighbor] := current
                        came_from.insert(std::make_pair<>(neighbor, current.location));

                        // A* LINE 24
                        // g_score[neighbor] := tentative_g_score
                        // A* LINE 25
                        // f_score[neighbor] := tentative_g_score + h(neighbor)
                        int f_new = tentative_g_score + calculate_h(neighbor);

                        // A* LINE 26
                        // openSet.add(neighbor)
                        auto neighbor_handle = open_set.emplace(
                            AStarNode(neighbor, tentative_g_score, f_new));
                        location_to_heaphandle.insert(std::make_pair<>(neighbor, neighbor_handle));
                        num_generated_nodes++;

                        cerr << "Add neighbor node (" << neighbor.y << ", " << neighbor.x << ") to open list" << endl;
                    }
                    // A* LINE 27
                    else // 在open_set中
                    {
                        auto neighbor_handle = pair_found->second;
                        // A* LINE 28
                        // if tentative_gScore < gScore[neighbor]
                        // meaning: This path to neighbor is better than any previous one. Record it!
                        if (tentative_g_score < (*neighbor_handle).g_score) // new path is better
                        {
                            // A* LINE 29
                            // came_from_list[neighbor] := current
                            came_from[neighbor] = current.location;

                            // A* LINE 30
                            // g_score[neighbor] := tentative_g_score
                            // A* LINE 31
                            // f_score[neighbor] := tentative_g_score + h(neighbor)

                            // A* LINE 32
                            // open_set.update(neighbor)
                            (*neighbor_handle).g_score = tentative_g_score;
                            (*neighbor_handle).f_score = tentative_g_score + calculate_h(neighbor);

                            open_set.increase(neighbor_handle);

                            cerr << "Update neighbor node (" << neighbor.y << ", " << neighbor.x << ") in open list" << endl;
                        }
                    }

                }
            }
        }

        iter++;
    }

    // A* LINE 33
    // Open set is empty but goal was never reached
    // return failure
    cerr << "Failed to find the Destination cell\n";
    return false;
}


void AStar::output_map()
{
    ofstream map_file("map.txt");
    if (map_file.is_open())
    {
        map_file << num_rows << " " << num_columns << "\n";
        cerr << "Parameter file created successfully.\n";

        for(int i=0;i<num_rows;i++)
        {
            for(int j=0;j<num_columns;j++)
            {
                if(obstacles.find(Location(j,i)) != obstacles.end())
                {
                    map_file << '@';
                }
                else
                {
                    map_file << '.';
                }

                if(j == num_columns-1)
                {
                    map_file << "\n";
                }
            }
        }

        map_file.close();
    }
    else
    {
        cerr << "Unable to create map file.\n";
    }
}


