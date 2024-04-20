//
// Created by take_ on 2023/8/2.
//

#ifndef DRAW_PATH_PY_CBS_H

#include <fstream>
#include <iostream>
#include <queue>
#include <unordered_map>
#include <utility>
#include <vector>

#include "low_level.hpp"
#include "util.hpp"


class ConstructorListNode
{
public:
    int cost;
    Location location;

public:
    bool operator<(const ConstructorListNode& other) const
    {
        if (cost != other.cost)
        {
            return cost > other.cost; // 把数值小的放在堆顶
        }
        else
        {
            if (location.y != other.location.y)
            {
                return location.y > other.location.y; // 把数值小的放在堆顶
            }
            else
            {
                return location.x > other.location.x; // 把数值小的放在堆顶
            }
        }
    }
};

// Node of constraint tree
class HighLevelNode
{
public:
    std::vector<std::vector<TimeLocation>> paths;
    size_t cost;
    std::vector<AgentConstraints> all_agents_constraints;

public:
    bool operator<(const HighLevelNode& other) const
    {
        return cost > other.cost; // 把数值小的放在堆顶
    }
};

class HighLevel
{
private:
    // IO var;
    std::string instance_filename;

    // data structure var;
    size_t num_rows;
    size_t num_columns;
    std::vector<std::vector<int>> map;
    std::unordered_set<Location> obstacles;

    int num_agents;
    std::vector<Location> starts;
    std::vector<Location> goals;
    std::vector<std::unordered_map<Location, int>> multi_agent_h_values;
    std::vector<std::vector<TimeLocation>> paths;

    // debug var;
    double start_time;
    int num_generated_high_level_nodes;
    int num_expanded_high_level_nodes;
    size_t num_expanded_low_level_nodes;
public:
    HighLevel(std::string input_filename); // constructor

    // Q: 页面中函数unordered_map<Location, int> compute_agent_h_values(Location input_goal)的功能是什么？
    // 函数的功能是计算一个启发式表，用于存储从地图上每个位置到目标位置(终点)的最短路径。
    // 函数使用了Dijkstra算法来构建一个以目标位置为根的最短路径树，然后遍历这棵树，把每个位置到目标位置的代价存储在一个字典中，并返回这个字典。
    // 这个启发式表可以用于A*算法中，来估计每个位置到目标位置的距离，从而提高搜索效率。
    // Q: 页面中函数def compute_heuristics(input_goal)输入参数的数据类型和含义分别是什么？
    // @param input_goal (Location): 目标节点的坐标
    // Q: 页面中函数def compute_heuristics(input_goal)输出结果的数据类型和含义分别是什么？
    // return h_values (unordered_map<Location, int>): 一个启发式表，字典中的每个键是一个Coordinate，表示一个节点坐标，
    // 字典中的每个值是一个整数，表示从该节点到目标节点的最短路径的代价。
    // 例如，{1: 4, 2: 3, 3: 2, 4: 0}表示一个启发式表，其中节点1到节点4的最短路径的代价是4，节点2到节点4的最短路径的代价是3，以此类推。
    std::unordered_map<Location, int> compute_agent_h_values(Location input_goal);

    // Q: 页面中函数Coordinate get_location(Path input_path, int input_time)的功能是什么？
    // 检测两条路径间是否有冲突(点冲突或边冲突)。
    // Q: 页面中函数Coordinate get_location(Path input_path, int input_time)输入参数的数据类型和含义分别是什么？
    // @param input_path (Path): 列表中的每个元素是一个表示位置的元组。
    // 例如，input_path = [(1, 2), (3, 4), (5, 6)]表示一个路径由三个位置组成，每个位置由两个整数表示。
    // @param input_time (int): 表示要查询的时间点。例如，2表示要查询路径上第二个位置。
    // Q: 页面中函数def get_location(input_path, input_time)输出结果的数据类型和含义分别是什么？
    // return (Location): 表示一个位置，包含两个整数，分别是横坐标和纵坐标。例如，(3, 4)表示横坐标为3，纵坐标为4的位置。
    static Location get_location(std::vector<TimeLocation> input_path, int input_time);

    // Q: 页面中函数vector<Conflict> get_all_paths_first_conflict(vector<Path> input_paths)的功能是什么？
    // 检测一组路径之间是否发生了任何碰撞，也就是多个智能体是否在同一个时间点占据了同一个位置或者交换了位置。
    // Q: 页面中函数vector<Conflict> get_all_paths_first_conflict(vector<Path> input_paths)输入参数的数据类型和含义分别是什么？
    // @param input_paths (vector<Path>): 所有的路径。
    // Q: 页面中函数vector<Conflict> get_all_paths_first_conflict(vector<Path> input_paths)输出结果的数据类型和含义分别是什么？
    // @return first_conflicts (vector<Conflict>): [{'a1': 1, 'a2': 2, 'location': [(1, 1), (0, 1)], 'time_step': 12},
    // {'a1': 1, 'a2': 3, 'location': [(3, 4), (3, 3)], 'time_step': 7},
    // {'a1': 1, 'a2': 4, 'location': [(3, 4)], 'time_step': 6},
    // {'a1': 2, 'a2': 3, 'location': [(3, 1), (2, 1)], 'time_step': 4},
    // {'a1': 2, 'a2': 4, 'location': [(3, 1)], 'time_step': 3}]
    static bool get_all_paths_first_conflict(std::vector<std::vector<TimeLocation>> input_paths, Conflict& first_conflict);

    // Q: 页面中函数vector<NegativeConstraint> generate_constraints_from_conflict(const Conflict& input_conflict)的功能是什么？
    // 根据一个碰撞，生成两个正向约束，用于重新规划两条路径，使得它们不再发生碰撞。
    // Q: 页面中函数vector<NegativeConstraint> generate_constraints_from_conflict(const Conflict& input_conflict)输入参数的数据类型和含义分别是什么？
    // @param conflict (Conflict): {'agent': 3, 'location': [(2, 0)], 'time_step': 2}
    // Q: 页面中函数vector<NegativeConstraint> generate_constraints_from_conflict(const Conflict& input_conflict)输出结果的数据类型和含义分别是什么？
    // @return multi_agent_constraints (vector<NegativeConstraint>): [{‘agent’: 0, ‘location’: [(3, 4)], ‘time_step’: 5},
    // {‘agent’: 1, ‘location’: [(3, 4)], ‘time_step’: 5}]
    static std::vector<NegativeConstraint> generate_constraints_from_conflict(const Conflict& input_conflict);

    // Q: 页面中函数bool high_level_search()的功能是什么？
    // 为多个智能体在地图上寻找不冲突的路径，使用了CBS算法的高层搜索。
    // Q: 页面中函数def find_solution()输入参数的数据类型和含义分别是什么？
    // None
    // Q: 页面中函数def find_solution()输出结果的数据类型和含义分别是什么？
    // @return void
    bool high_level_search();

    // debug func
    std::vector<std::vector<int>> get_map();
    void print_map();
    static void print_constructor_list_node(ConstructorListNode input_node);
    static void print_constructor_list(std::priority_queue<ConstructorListNode> input_list);
    static void print_agent_h_values(const std::unordered_map<Location, int>& input_h_values);
    void print_multi_agent_h_values();
    static void print_path(const std::vector<TimeLocation>& input_path);
    static void print_node_paths(HighLevelNode input_node);
};

#define DRAW_PATH_PY_CBS_H

#endif //DRAW_PATH_PY_CBS_H
